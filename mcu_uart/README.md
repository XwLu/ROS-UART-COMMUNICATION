## UART_Communication(ros-stm32)
## platform: stm32F103 RBT6

###协议内容：
> #### ff ff 01 00 02 01 01 05
> #### 数据头(ff ff) 控制对象编号(01) 数据长度(00 02) 设备编号(01) 执行内容(01) 校验和位(05)
