## docker


#### centos

######	移除老的docker
	yum remove docker  docker-common docker-selinux docker-engine

######	查看可用版本有哪些
	yum list docker-ce --showduplicates | sort -r

######	选择一个版本并安装
	yum install docker-ce-版本号

######	启动 Docker 并设置开机自启
	systemctl start docker
	systemctl enable docker

######	docker基本操作


######  docker 对多个端口映射
	docker run --name ubuntu_server  -itd -p 8888:22 -p 8889-8910:8889-8910  ubuntu:22.04 /bin/bash