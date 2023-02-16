To set proxy in bash and zsh, we need to code the bash script or zsh script.
We will edit the .bashrc or .zshrc dot file in your home path like:
```bash
/home/yourname/.bashrc
or
/home/yourname/.zshrc
```
Then, use neovim (maybe you can chose your favourite editor whatever :D ) to edit:
```bash
$ nvim ~/.bashrc
```
or
```bash
$ nvim ~/.zshrc
```
Add the proxy and noproxy function:
```bash
proxy(){
	export all_proxy=socks5://localhost:7890
	echo "All proxy On!"
}
noproxy(){
	unset all_proxy
	echo "All proxy Off!"
}
```
After the bash command `source ~/.bashrc`, you can use `proxy` command to set the proxy on. Also use `noproxy` command to set the proxy off.
![image.png](https://cdn.jsdelivr.net/gh/InverseDa/image@master/image/20230201214625.png)

You can `echo "proxy" > ~/.zshrc` if you like to set proxy when open the terminal (Just like python aha). 