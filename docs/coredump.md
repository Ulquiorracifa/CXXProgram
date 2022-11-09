###	 修改ulimit限制 
	vim /etc/profile 然后，在profile中添加：
	ulimit -c unlimited
	source /etc/profile

### 修改coredump生成位置
	cat /proc/sys/kernel/core_pattern
	/sbin/sysctl -w kernel.core_pattern=/var/dump/%e.core.%p
	
	/etc/sysctl.conf 
	kernel.core_pattern = /var/core/core_%e_%p
	kernel.core_uses_pid = 0
	
	为了更详尽的记录core dump当时的系统状态，可通过以下参数来丰富core文件的命名：
	%% 单个%字符
	%p 所dump进程的进程ID
	%u 所dump进程的实际用户ID
	%g 所dump进程的实际组ID
	%s 导致本次core dump的信号
	%t core dump的时间 (由1970年1月1日计起的秒数)
	%h 主机名
	%e 程序文件名

### 安装debuginfo
		先修改"/etc/yum.repos.d/CentOS-Debuginfo.repo"文件的 enable=1；有时候该文件不存在，则需要手工创建此文件并加入以下内容：
	touch /etc/yum.repos.d/CentOS-Debuginfo.repo
	vim /etc/yum.repos.d/CentOS-Debuginfo.repo
	
	[debug]
	name=CentOS-7 - Debuginfo
	baseurl=http://debuginfo.centos.org/7/$basearch/ 
	gpgcheck=1
	gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-Debug-7 
	enabled=1
	
	sudo yum install -y glibc
	debuginfo-install glibc
	debuginfo-install libgcc


​	