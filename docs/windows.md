###	Windows开发环境搭建

######	 1.安装Visual Studio 2019及以上
######	2.安装Git版本2.9及以上
######	3.安装CMake版本3.23及以上
######	4.安装vcpkg
    > 子模块vcpkg下载
        git submodule update --init --recursive
    > 初始化vcpkg
        .\third_party\vcpkg\bootstrap-vcpkg.bat
######	5.通过vcpkg安装库
	> 设置环境变量,选择安装64位版本
		set VCPKG_DEFAULT_TRIPLET=x64-windows
	> 安装库文件
		>> 执行命令安装
			cd ./third_party/vcpkg/
			vcpkg install hiredis protobuf
		>> 执行脚本安装
			./vcpkg_install.bat
