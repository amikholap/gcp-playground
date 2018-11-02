GCPPG_BUILD_IMAGE = "carabas/gcppg-build:latest"
GCPPG_IMAGE = "carabas/gcppg:latest"

.PHONY: build bake-latest-build push-latest

all: build bake-latest-build push-latest

build:
	docker build --file=Dockerfile.build --tag=${GCPPG_BUILD_IMAGE} .

bake-latest-build:
	$(eval TMP_DIR = $(shell mktemp -d -t gcppg))
	echo "baking workdir: ${TMP_DIR}"
	docker run --rm -v ${TMP_DIR}:/host ${GCPPG_BUILD_IMAGE} cp /opt/gcppg /host/gcppg
	cp Dockerfile ${TMP_DIR}
	cd ${TMP_DIR} && docker build --file=Dockerfile --tag=${GCPPG_IMAGE} .
	rm -rf ${TMP_DIR}

push-latest:
	docker push ${GCPPG_IMAGE}