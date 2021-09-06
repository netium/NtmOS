#include "tasks.h"
#include "kern_basic.h"
#include "gui.h"
#include "kernel_functions.h"
#include "segments.h"
#include "interrupt_handlers.h"
#include "kernel_inits.h"
#include "k_heap.h"
#include "synchron.h"

// We will have a idle task, this task will always use the GDT slot 3;
// For other tasks, we will allocate the GDT slot dynamically

tss_t g_tss3 __attribute__((aligned(4)));
tss_t g_tss4 __attribute__((aligned(4)));

#define N_MAX_PROCESSES (64)
#define N_TSS_START_SLOT_IN_GDT (16)
#define N_LDT_SEG_START_SLOT_IN_GDT (80)

static process_t s_processes[N_MAX_PROCESSES] __attribute__((aligned(4))) = {0};

process_t *g_current_task = 0;
process_t *g_idle_task = 0;
process_t *g_ready_task_queue_head = 0;
process_t **g_ready_task_queue_tail_pointer = &g_ready_task_queue_head;
process_t *g_sleep_task_queue_head = 0;
process_t *g_finished_task_queue = 0;
process_t *g_io_blocked_task_queue_head = 0;

// simple userspace code for test
unsigned char simpleapp_data[] = {0x31, 0xc0, 0x40, 0xeb, 0xfb};

timer_t *g_task_switch_timer = 0;

static volatile int g_next_task_id = 1;

int set_task_into_gdt(unsigned int slot, void *base_addr, unsigned int limit);

void idle_task_main(process_t *task);
void task_main(process_t *task);
void initial_idle_task();
unsigned int initial_default_task();
void put_task_to_ready(process_t *task);

void process_keyboard_event(process_t *task, keyboard_event_t *process_keyboard_event);

void process_mouse_event(process_t *task, mouse_event_t *p_mouse_event);

void process_timer_event(process_t *task, timer_event_t *p_timer_event);

void init_process_management() {
	for (int i = 0; i < N_MAX_PROCESSES; i++) {
		s_processes[i].tss_entry_id = i;
		s_processes[i].status = TASK_UNUSED;

		// Install the task's TSS to the GDT
		install_tss_to_gdt(N_TSS_START_SLOT_IN_GDT + i, &s_processes[i].tss, sizeof(s_processes[i].tss));

		// Install the task's LDT to the GDT
		install_ldt_to_gdt(N_LDT_SEG_START_SLOT_IN_GDT + i, &s_processes[i].ldt, sizeof(s_processes[i].ldt));

		// Initial process task LDT table
		install_seg_to_ldt(&s_processes[i].ldt, 0, 0x4000000 + (i * 0x100000), 0x100000);
		install_seg_to_ldt(&s_processes[i].ldt, 1, 0x4000000 + (i * 0x100000), 0x100000);

		// Set the TSS ldtr value
		s_processes[i].tss.ldtr = (N_LDT_SEG_START_SLOT_IN_GDT + i) << 3;
	}
}

void idle_task_main(process_t *task)
{
	sti();
	while (1)
	{
		halt();
	}
}

void task_main(process_t *task)
{
	char msg[256];
	k_sprintf(msg, "Task %x is started", (unsigned int)task);

	k_printf(msg);

	sti();

	while (1)
	{
		cli(); // Temporarily disable the interrupt, to prevent system from re-entry the manipulation of the event queue
		simple_interrupt_event_node_t *node = dequeue_event_queue(&task->event_queue);

		if (node == 0)
		{
			sti_halt();
		}
		else
		{
			sti();
			switch (node->type)
			{
			case KEYBOARD_EVENT:
				process_keyboard_event(task, &(node->keyboard_event));
				break;
			case MOUSE_EVENT:
				process_mouse_event(task, &(node->mouse_event));
				break;
			case TIMER_EVENT:
				process_timer_event(task, &(node->timer_event));
				break;
			default:
				break;
			}
			k_free(node);
		}
	}
}

void switch_task(timer_t *timer)
{
	int eflags;
	get_eflags(eflags);
	cli();
	k_printf("Start to switch process...");
	process_t *cur_task = g_current_task;
	unsigned int tss_id = cur_task->tss_entry_id;

	process_t *sched_task = g_ready_task_queue_head;
	if (0 == sched_task)
	{ // No any ready to run task, continue to run
		k_printf("No new task in ready queue, continue the current task....");
		k_set_timer_time(timer, 1000);
		set_eflags(eflags);
		return;
	}
	else
	{ // Pick the first ready task out from the ready queue and adjust head & tail accordingly
		g_ready_task_queue_head = sched_task->next;
		if (g_ready_task_queue_head == 0)
			g_ready_task_queue_tail_pointer = &g_ready_task_queue_head;
	}

	cur_task->status = TASK_READY;
	if (cur_task != g_idle_task)
	{
		// If current task is not idle task, then put it to the ready queue
		*g_ready_task_queue_tail_pointer = cur_task;
		g_ready_task_queue_tail_pointer = &cur_task->next;
	}

	sched_task->status = TASK_RUNNING;

	sched_task->next = 0x0;

	g_current_task = sched_task;

	k_set_timer_time(timer, 1000);

	char msg[256];
	k_sprintf(msg, "Switch to task at %x for process slot id: %x", (unsigned int)sched_task, sched_task->tss_entry_id);
	k_printf(msg);

	k_itoa(sched_task->pid, msg, 16);
	k_printf(msg);

	set_eflags(eflags);

	_switch_task(0, (N_TSS_START_SLOT_IN_GDT + g_current_task->tss_entry_id) << 3);
}

process_t *task_alloc()
{
	process_t *task = NULL;

	// Find the unused task slot
	for (int i = 0; i < N_MAX_PROCESSES; i++) {
		if (s_processes[i].status == TASK_UNUSED) {
			task  = &s_processes[i];
			break;
		}
	}

	if (NULL != task)
	{
		task->status = TASK_ALLOC;
		task->pid = g_next_task_id;
		atom_inc(g_next_task_id);
	}

	return task;
}

void task_free(process_t *task)
{
	if (0 == task)
		_panic();	// task pointer shall not be NULL

	if (task->pid == 0)
		_panic();	// shall not free an unused task slot

	task->pid = 0;

	task->status = TASK_UNUSED;

	if (0 != task->kern_stack)
		k_free(task->kern_stack);

	if (0 != task->data_stack)
		k_free(task->data_stack);
}

void task_init(process_t *task, int data_size, int kern_stack_size)
{
	if (NULL == task || TASK_ALLOC != task->status)
		return;

	// Currently not limit the data region
	// task->data = k_malloc(data_size);
	// task->data_size = data_size;

	task->kern_stack = k_malloc(kern_stack_size);
	task->kern_stack_size = kern_stack_size;

	task->pid = g_next_task_id;
	atom_inc(g_next_task_id);

	task->console = 0;

	task->next = 0;

	initial_task_event_queue(&task->event_queue);
	task->tss.iopb_offset = 0x40000000;
	task->tss.eflags = 0x202;
	task->tss.eax = 0x0;
	task->tss.ebx = 0x0;
	task->tss.ecx = 0x0;
	task->tss.edx = 0x0;
	task->tss.ebp = 0x0;
	task->tss.esi = 0x0;
	task->tss.edi = 0x0;
	task->tss.cs = 0x1 << 3;
	task->tss.ds = 0x2 << 3;
	task->tss.ss = 0x2 << 3;
	task->tss.es = 0x2 << 3;
	task->tss.fs = 0x2 << 3;
	task->tss.gs = 0x2 << 3;
	task->tss.cr3 = (size_t)GPT_TABLE_PHY_START_ADDR;
	task->tss.esp = (unsigned int)(task->kern_stack) + task->kern_stack_size - 4;
	task->tss.eip = 0x0; // Temporarily set to 0x0, but will overrided to the new task start address

	task->tss.esp0 = (0x1 << 3);
	task->tss.ss0 = task->tss.ss;

	if (g_current_task > 0)
		task->ppid = g_current_task->pid;
	else
		task->ppid = 0;

	task->status = TASK_INIT;

	// Hook: to copy the sample app code to the userspace memory:
	k_memcpy((void *)get_proc_umem_start_address(task->tss_entry_id), simpleapp_data, sizeof(simpleapp_data));	
}

void initial_task_event_queue(simple_interrupt_event_queue_t *event_queue)
{
	if (0 == event_queue)
		return;
	event_queue->head = 0;
	event_queue->tail = 0;
	event_queue->full = 0;
}

void start_task(process_t *task, void *start_addr)
{
	if (0 == task || 0 == start_addr)
		return;
	if (TASK_INIT != task->status)
		return;

	// Push task control block pointer to the task stack
	task->tss.esp -= 4;
	*(unsigned int *)task->tss.esp = (unsigned int)task;

	// Push fake return address to the task stack
	task->tss.esp -= 4;
	*(unsigned int *)task->tss.esp = (unsigned int)0;

	task->tss.eip = (unsigned int)start_addr;

	put_task_to_ready(task);
}

void put_task_to_ready(process_t *task)
{
	if (0 == task)
		return;
	task->status = TASK_READY;

	*g_ready_task_queue_tail_pointer = task;
	g_ready_task_queue_tail_pointer = &task->next;
}

process_t *initial_tasks()
{

	initial_idle_task();				// Initial the idle task
	unsigned int gdt_slot = initial_default_task(); // Initial the default task (which is the current task)

	_set_tr(gdt_slot << 3);

	process_t *current_task = g_current_task;

	g_task_switch_timer = k_timer_alloc();
	k_init_timer(g_task_switch_timer, switch_task, 100);
	k_set_timer_time(g_task_switch_timer, 1000);

	return current_task;
}

void initial_idle_task()
{
	// temp comments: The following function will corrupt the virt mem page directory,
	// it shall be be cause the heap has data over flow to the page directory area.
	process_t *idle_task = task_alloc();
	if (0 == idle_task)
		_panic();
	task_init(idle_task, 64 * 1024, 64 * 1024);
	g_idle_task = idle_task;
	g_idle_task->status = TASK_READY;
	g_idle_task->tss.eip = (unsigned int)idle_task_main;
}

unsigned int initial_default_task()
{
	process_t *task = task_alloc();
	if (0 == task)
		_panic();

	task->kern_stack_size = 1024 * 1024;

	task->pid = 0;

	task->console = 0;

	task->next = 0;

	initial_task_event_queue(&task->event_queue);

	task->tss.iopb_offset = 0x40000000;
	task->tss.eflags = 0x202;
	task->tss.eax = 0x0;
	task->tss.ebx = 0x0;
	task->tss.ecx = 0x0;
	task->tss.edx = 0x0;
	task->tss.ebp = 0x0;
	task->tss.esi = 0x0;
	task->tss.edi = 0x0;
	task->tss.cs = 0x1 << 3;
	task->tss.ds = 0x2 << 3;
	task->tss.ss = 0x2 << 3;
	task->tss.es = 0x2 << 3;
	task->tss.fs = 0x2 << 3;
	task->tss.gs = 0x2 << 3;
	task->tss.esp = 0;
	task->tss.eip = 0x0;
	task->tss.cr3 = (size_t)GPT_TABLE_PHY_START_ADDR;
	task->tss.ss0 = task->tss.ss;
	task->tss.esp0 = task->tss.esp;

	task->ppid = 0;
	task->pid = g_next_task_id;
	atom_inc(g_next_task_id);

	task->status = TASK_RUNNING;

	g_current_task = task;

	// Hook: to copy the sample app code to the userspace memory:
	k_memcpy((void *)get_proc_umem_start_address(task->tss_entry_id), simpleapp_data, sizeof(simpleapp_data));	
	unsigned short ldtr = (N_LDT_SEG_START_SLOT_IN_GDT + task->tss_entry_id) << 3;

	load_ldt(ldtr);

	return N_TSS_START_SLOT_IN_GDT + task->tss_entry_id;
}

int enqueue_event_queue(simple_interrupt_event_queue_t *queue, simple_interrupt_event_node_t *p_node)
{
	if (0 == queue || 0 == p_node)
		return -1;

	if (queue->full)
		return -1;

	queue->nodes[queue->tail++] = p_node;

	if (queue->tail >= (sizeof(queue->nodes) / sizeof(queue->nodes[0])))
		queue->tail = 0;

	if (queue->head == queue->tail)
		queue->full = 1;

	return 0;
}

simple_interrupt_event_node_t *dequeue_event_queue(simple_interrupt_event_queue_t *queue)
{
	if (0 == queue)
		return 0;

	if ((queue->head == queue->tail) && (queue->full == 0))
		return 0; // means the queue is empty

	simple_interrupt_event_node_t *node = queue->nodes[queue->head++];

	if (queue->head >= (sizeof(queue->nodes) / sizeof(queue->nodes[0])))
		queue->head = 0;

	queue->full = 0;

	return node;
}

void process_keyboard_event(process_t *task, keyboard_event_t *process_keyboard_event)
{
	tui_putchar(process_keyboard_event->data);
	if (process_keyboard_event->data == 'A')
		__asm__ inline volatile("int %0" ::"N"(0x80));
}

void process_mouse_event(process_t *task, mouse_event_t *p_mouse_event)
{
}

void process_timer_event(process_t *task, timer_event_t *p_timer_event)
{
	if (0 == p_timer_event)
		return;

	if (0 != p_timer_event->p_timer->pf)
	{
		p_timer_event->p_timer->pf(p_timer_event->p_timer);
	}
}

process_t* kern_fork(process_t *source) {
	if (NULL == source)
		_panic();

	process_t * new = task_alloc();
	task_init(new, source->data_size, source->kern_stack);

	k_memcpy(new->data_stack, source->data_size, source->data_size);		
}