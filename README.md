# 获取国内IP、域名

不想自己生成的可以直接下载
* https://github.com/HMBSbige/GetCNDomainsAndIPv4/tree/master/out
* 部分ShadowsocksR文件：https://github.com/HMBSbige/Text_Translation/tree/master/ShadowsocksR

所需文件：

* [delegated-apnic-latest](https://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest)
* [gfwlist.txt](https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt)
* [accelerated-domains.china.conf](https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/accelerated-domains.china.conf)
* (可选)gfwlist自定义规则文件 user-rule.txt
* (可选)不想被解析的域名名单 whitelist.txt

## 输出文件用处
__chn_ip.txt__：

直接放进ShadowsocksR目录内,代理规则选择"绕过局域网和大陆"

~~其实将delegated-apnic-latest.txt放入ShadowsocksR根目录后重启ShadowsocksR也可以自己生成~~

__add.txt__：

使用以下bat添加路由表，与[cmroute.dll](https://github.com/HMBSbige/getcnIP/releases/download/1.0/cmroute.dll)同目录
```
@echo off

net session >nul 2>&1
if not %errorlevel% == 0 (
  echo 请使用右键 "已管理员身份运行" 此脚本
  pause
  exit 1
)

cd /d %~dp0
echo 导入路由表...
rundll32.exe cmroute.dll,SetRoutes /STATIC_FILE_NAME add.txt /DONT_REQUIRE_URL /IPHLPAPI_ACCESS_DENIED_OK
pause
```
__del.txt__：

使用以下bat删除路由表，与[cmroute.dll](https://github.com/HMBSbige/getcnIP/releases/download/1.0/cmroute.dll)同目录
```
@echo off

net session >nul 2>&1
if not %errorlevel% == 0 (
  echo 请使用右键 "已管理员身份运行" 此脚本
  pause
  exit 1
)

cd /d %~dp0
echo 移除路由表...
rundll32.exe cmroute.dll,SetRoutes /STATIC_FILE_NAME del.txt /DONT_REQUIRE_URL /IPHLPAPI_ACCESS_DENIED_OK
pause
```
注：重启系统后自动清空路由表

__gfwlist.pac__：

ShadowsocksR目录内的pac.txt。

~~其实用SSR自己生成的就行了~~

__CNdomains.txt__：

https://github.com/bitbeans/SimpleDnsCrypt

可以用来一键自建Windows的DNS服务器

给这软件里的屏蔽域名用的。

把国内能正常解析的域名都屏蔽了，只解析国外域名，用备用DNS来解析国内域名。

__user.rule__：

放进ShadowsocksR目录内,代理规则选择"用户自定义"

本程序生成的规则配合使用全局模式可绕过国内域名和ip，只代理国外的。

~~并没有什么用，不知道为什么**响应时间太长**，还是用pac模式快~~

https://github.com/HMBSbige/shadowsocks-rss/wiki/C%23-Proxy-Rule

__ss_cnip.pac__：

ShadowsocksR目录内的pac.txt。

上面user.rule的替代品，客户端设置为PAC模式，代理规则选择全局。

可绕过国内域名和国内ip。

__ss_white.pac__：

ShadowsocksR目录内的pac.txt。

可绕过国内域名。

__ss_white_r.pac__：

ShadowsocksR目录内的pac.txt。

仅通过国内域名（国外访问大陆）。

__proxy.pac__：

自用。

ShadowsocksR选择"保持当前状态不修改"。将Internet属性里的自动配置脚本的地址设为本地路径。

只是觉得IDM里的代理设置总是要改太麻烦，然而好像IDM里的自动配置脚本不支持本地路径？

__whitelist.acl__

ShadowsocksR 安卓客户端用ACL文件

## 自用脚本
```
aria2c -x10 -s10 --https-proxy="https://127.0.0.1:23333" --allow-overwrite=true -Z https://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/accelerated-domains.china.conf
getcnip
pause
```

## 关于cmroute.dll
我能找到的文档：https://technet.microsoft.com/zh-cn/library/dd672665(v=ws.10).aspx

但是文档实在太老，很多都不适用。而且不知道为什么win10提取的cmroute.dll不能用...

简单的来说，

* 基本格式：

> 命令 网络目标 MASK 网络掩码 网关 METRIC 跃点数 IF 接口

* 命令可以是:

	* ADD
	* DELETE
	* REMOVE_GATEWAY


* 网关 跃点数 接口一般都默认（default）


* 用`route print` 命令查看 __接口列表__

* METRIC __不可__ 省略，且不知道为什么跃点数只能是default，填数字无效。
