Git使用
===
# 1 配置
## 1.1 中文转义问题
```
$ git config --global core.quotepath false
```
## 1.2 github代理加速
```
1. 打开加速
git config --global http.proxy 'socks5://127.0.0.1:1080'
git config --global https.proxy 'socks5://127.0.0.1:1080'

2. 关闭加速
git config --global --unset http.proxy
git config --global --unset https.proxy
```
## 1.3 git的URL特殊字符转义
```
空格	-	%20
"		-	%22
#		-   %23
%		-   %25
&		-   %26
(		-   %28
)       -   %29
+       -   %2B
,		-   %2C
/		-   %2F
:		-   %3A
;		-   %3B
<		-   %3C
=		-   %3D
>		-   %3E
?		-   %3F
@		-   %40
\       -   %5C
|       -   %7C
```
# 2 技巧

