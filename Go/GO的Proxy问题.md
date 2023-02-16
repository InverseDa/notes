# Go的Proxy问题

- 设置GOPROXY代理：

  ```bash
  go env -w GOPROXY=https://goproxy.cn,direct
  ```

- 设置GOPRIVATE来跳过私有库，比如常用的Gitlab或Gitee，中间使用逗号分隔：

  ```bash
  go env -w GOPRIVATE=*.gitlab.com,*.gitee.com
  ```

  