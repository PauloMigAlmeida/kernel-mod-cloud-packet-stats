### Automatic Makefile generation by 'genmake' script        ####
### Copyright, Jerry Cooperstein, coop@linuxfoundation.org 2/2003 - 1/2017 ####
### License: GPLv2 ###

export KROOT=/lib/modules/$(shell uname -r)/build

obj-m +=  kernel_cloud_packet_stats.o

kernel_cloud_packet_stats-y :=  module.o 			\
		  						sysfs.o 			\
		  						packetcounter.o 	\
		  						netfilter.o 		\
		  						cidr.o 		\

allofit:  modules
source_prep_create:
	mkdir -p ./source_prep_build_tmp
	./script/gcp_get_ip_range.sh > ./source_prep_build_tmp/gcp.txt
	./script/aws_get_ip_range.sh > ./source_prep_build_tmp/aws.txt
	./script/azure_get_ip_range.sh > ./source_prep_build_tmp/azure.txt
	touch ./cloud_cidr_block.in	
source_prep_build: source_prep_create
	echo "" > ./cloud_cidr_block.in	
	python3 ./script/process_cidr.py ./source_prep_build_tmp/gcp.txt >> ./cloud_cidr_block.in
	python3 ./script/process_cidr.py ./source_prep_build_tmp/aws.txt >> ./cloud_cidr_block.in
	python3 ./script/process_cidr.py ./source_prep_build_tmp/azure.txt >> ./cloud_cidr_block.in
source_prep_clean:
	rm -rf ./source_prep_build_tmp
	rm -f ./cloud_cidr_block.in
modules: source_prep_build
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules
modules_install:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install
kernel_clean:
	@$(MAKE) -C $(KROOT) M=$(shell pwd) clean
clean: kernel_clean source_prep_clean
	rm -rf   Module.symvers modules.order
