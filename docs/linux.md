### 		Linux开发环境搭建

######	1.升级 GCC 至8.0
	> Centos
	>> 安装GCC
		sudo yum install centos-release-scl -y
		sudo yum install devtoolset-8 -y
	>> 在当前bash中开启GCC8
		source /opt/rh/devtoolset-8/enable
	>> 开启GCC8命令写入到bashrc中
		echo "source /opt/rh/devtoolset-8/enable" >> /etc/bashrc
		source /etc/bashrc 
	> ubuntu
	>> 安装GCC
		sudo apt install build-essential -y 
		apt install g++ gdb make ninja-build rsync zip curl

######	2.安装Git版本2.9及以上
	> 下载
	    wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.9.5.tar.gz
	    tar -xvf git-2.9.5.tar.gz
	    cd git-2.9.5
	> 安全libcurl
		>> 不安装就会缺少https的功能
		apt install libcurl4-openssl-dev
	> 安装
	    ./configure --prefix=/usr/local/git all
	    make (此处不要加-jx,会编译失败)
	    make install
	    >> centos
			安装报错make[1]: *** [perl.mak]    yum install perl-ExtUtils-MakeMaker package
		>> ubuntu
			缺少zlib  apt-get install zlib1g.dev
			缺少tcl po/bg.msg make[1]: *** [Makefile:250: po/bg.msg]  apt install tcl
			缺少gettext [po/build/locale/bg/LC_MESSAGES/git.mo] 错误 127 apt install 
	> 配置环境变量
	    echo "export PATH=$PATH:/usr/local/git/bin" >> /etc/profile
	    source /etc/profile
	> 查看结果
	    git --version
	    git version 2.9.5

######	3.安装CMake版本3.23及以上
	> 下载
		wget https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2.tar.gz
		tar -xvf cmake-3.23.2.tar.gz
		cd cmake-3.23.2
	> 安装
		./bootstrap --prefix=/usr/local/cmake
		make
		make install
		>> ubuntu
			缺少libssl apt-get install libssl-dev
	> 配置环境变量
		echo "export PATH=$PATH:/usr/local/cmake/bin" >> /etc/profile
		source /etc/profile
	> 查看结果
		cmake --version
		cmake version 3.23.2

######	4.安装vcpkg
	> 子模块vcpkg下载
		git submodule update --init --recursive
	> 初始化vcpkg
		sh ./third_party/vcpkg/bootstrap-vcpkg.sh

######	5.通过vcpkg安装库
	> 设置环境变量,选择安装64位版本
		export VCPKG_DEFAULT_TRIPLET=x64-linux
	> 安装库文件
		>> 执行命令安装
			cd /third_party/vcpkg/
			vcpkg install hiredis protobuf
		>> 执行脚本安装
			sh /vcpkg_install.sh

######	6.编译相关
	> 缺少asan
		>> centos
			根据gcc版本安装对应的asan版本
			yum install devtoolset-8-libasan-devel
		>> ubuntu
			apt install pkg-config


​			
######	7.wsl
	> wsl切换成root用户登录
