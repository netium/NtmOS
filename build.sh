
docker run --rm -v "$(pwd):/var/ntmos" netium/ntmos-build:latest

docker run --rm --privileged=true -v "$(pwd):/var/ntmos" netium/ntmos-build:latest "/usr/bin/bash" "/var/ntmos/build_hdd_image.sh"

exit $?