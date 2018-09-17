all: awss_drv
	gcc awss_drv.c -o awss_drv
clean:
	rm awss_drv
