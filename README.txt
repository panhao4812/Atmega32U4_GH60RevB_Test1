Atmega32U4_GH60

精简版固件 
特殊换层模式
矩阵灯
附带上位机
-------------------------------------
1 6pin或者dfu或者avr109（arduino micro）刷机，不需要改fuse
---------------------------------------
hidraw发送格式 interface 2 endpoint 4
8byte 50ms 无需处理raw回复信息 
如果没有接收响应则会卡死 拔掉usb插头即可
发送数据都是16位数
FF F1 00 00 00 00 00 00 //开始写eep
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
(uint16)地址 (uint16)data1 (uint16)data2 (uint16)data3
...
FF F2 00 00 00 00 00 00 //结束写eep
---------------------------------------
eep格式
前10byte是5个16位数：以row=5 col=14为例
 row地址 ，col地址 ，矩阵1地址 ，矩阵2地址 ，矩阵mask地址
// 10    10+5=15   10+5+14=29   10+5+14+70=99  10+5+14+140=169
其它信息都是8位数：
row[5] row的IO
col[14] col的IO
hexakey0[5,14] layer0矩阵
hexakey1[5,14] layer1矩阵
keymask[5,14]  矩阵标识
---------------------------------------
矩阵标识 
8位标识表示2层矩阵 16位标识表示4层矩阵
//keymask（8bit）: 7 -press 6,5,4   -hexatype0 3 -press 2,1,0 -hexatype1
//keymask（16bit）:15-press 14,13,12-hexatype0 11-press 10,9,8-hexatype1
                   7 -press 6,5,4   -hexatype2 3 -press 2,1,0 -hexatype3
7种不同类型的输入
hexatype（3bit）: 1-key 2-modifykey 3-mousekey 4-systemkey 5-consumerkey 6-FN 7-macro
用法：
uint8_t FN;
判断mask&FN的值
//keymask（8bit） :mask&0xF0 或者 mask&0x0F 来切换层
//keymask（16bit）:mask&0xF000, mask&0x0F00, mask&0x00F0, mask&0x000F 来切换层
固定层即固定FN的值

瞬时切换是base层->目标层
层翻页是base层+1
-------------------------------------
按键延时
延时按矩阵循环的周期来确定，不用delay
uint8_t delay_after=0;后摇
uint8_t delay_before=0;前摇
按键的时候会给delay_after赋值（3-8）delay_after！=0表示处于按下状态
判断buffer有变化的时候给delay_before赋值（2-4）delay_before！=0表示buffer变化了
if(delay_after>0)delay_after-=1;
	
if(delay_before>0)delay_before-=1;
用法：
delay_after==4 && delay_before==1表示按下去之后抖动停止（1-10ms左右）
delay_after==1表示松开之后抖动停止
--------------------------------------
RGB
直接读色彩字典。字典由grasshopper生成
C# capsule 1
 private void RunScript(object x, object y, ref object A, ref object B, ref object C)
  {
    List<int> index = new List<int>();
    for(int i = 0;i < 0x0200;i++){
      index.Add(i);
    }
    A = 0;B = 0x0200;
    C = index;
  }
中间接一个gradient capsule
C# capsule 2
private void RunScript(List<Color> x, object y, ref object A, ref object B, ref object C)
  {
    string Red="{",Green = "{",Blue = "{";
    for(int i = 0;i < x.Count;i++){
      Red += x[i].R.ToString();
      if(i < x.Count - 1)Red += ",";
      if((i + 1) % 25 == 0)Red += "\r\n";
      Green += x[i].G.ToString();
      if(i < x.Count - 1)Green += ",";
      if((i + 1) % 25 == 0)Green += "\r\n";
      Blue += x[i].B.ToString();
      if(i < x.Count - 1)Blue += ",";
      if((i + 1) % 25 == 0)Blue += "\r\n";
    }
    Red += "}";Green += "}";Blue += "}";
    A = Red;
    B = Green;
    C = Blue;
  }

参考usbpcap
数据头USB URP 28长度

Host： GET DESCRIPTOR Request DEVICE
Setup Data
    bmRequestType: 0x80
        1... .... = Direction: Device-to-host
        .00. .... = Type: Standard (0x0)
        ...0 0000 = Recipient: Device (0x00)
    bRequest: GET DESCRIPTOR (6)
    Descriptor Index: 0x00
    bDescriptorType: DEVICE (0x01)
    Language Id: no language specified (0x0000)
wLength: 18

Dvice： GET DESCRIPTOR Response DEVICE
DEVICE DESCRIPTOR
    bLength: 18
    bDescriptorType: 0x01 (DEVICE)
    bcdUSB: 0x0200
    bDeviceClass: Device (0x00)
    bDeviceSubClass: 0
    bDeviceProtocol: 0 (Use class code info from Interface Descriptors)
    bMaxPacketSize0: 8
    idVendor: Razer USA, Ltd (0x1532)
    idProduct: RZ01-0036 Optical Gaming Mouse [Abyssus] (0x001c)
    bcdDevice: 0x0100
    iManufacturer: 1
    iProduct: 2
    iSerialNumber: 0
bNumConfigurations: 1

GET DESCRIPTOR Request CONFIGURATION
Setup Data
    bmRequestType: 0x80
        1... .... = Direction: Device-to-host
        .00. .... = Type: Standard (0x0)
        ...0 0000 = Recipient: Device (0x00)
    bRequest: GET DESCRIPTOR (6)
    Descriptor Index: 0x00
    bDescriptorType: CONFIGURATION (0x02)
    Language Id: no language specified (0x0000)
    wLength: 9

GET DESCRIPTOR Response CONFIGURATION
CONFIGURATION DESCRIPTOR
    bLength: 9
    bDescriptorType: 0x02 (CONFIGURATION)
    wTotalLength: 34
    bNumInterfaces: 1
    bConfigurationValue: 1
    iConfiguration: 0
    Configuration bmAttributes: 0xa0  NOT SELF-POWERED  REMOTE-WAKEUP
bMaxPower: 50  (100mA)

GET DESCRIPTOR Request CONFIGURATION
Setup Data
    bmRequestType: 0x80
        1... .... = Direction: Device-to-host
        .00. .... = Type: Standard (0x0)
        ...0 0000 = Recipient: Device (0x00)
    bRequest: GET DESCRIPTOR (6)
    Descriptor Index: 0x00
    bDescriptorType: CONFIGURATION (0x02)
    Language Id: no language specified (0x0000)
    wLength: 34

GET DESCRIPTOR Response CONFIGURATION
CONFIGURATION DESCRIPTOR
    bLength: 9
    bDescriptorType: 0x02 (CONFIGURATION)
    wTotalLength: 34
    bNumInterfaces: 1
    bConfigurationValue: 1
    iConfiguration: 0
    Configuration bmAttributes: 0xa0  NOT SELF-POWERED  REMOTE-WAKEUP
    bMaxPower: 50  (100mA)
INTERFACE DESCRIPTOR (0.0): class HID
    bLength: 9
    bDescriptorType: 0x04 (INTERFACE)
    bInterfaceNumber: 0
    bAlternateSetting: 0
    bNumEndpoints: 1
    bInterfaceClass: HID (0x03)
    bInterfaceSubClass: Boot Interface (0x01)
    bInterfaceProtocol: Mouse (0x02)
    iInterface: 0
HID DESCRIPTOR
    bLength: 9
    bDescriptorType: 0x21 (HID)
    bcdHID: 0x0111
    bCountryCode: Not Supported (0x00)
    bNumDescriptors: 1
    bDescriptorType: HID Report (0x22)
    wDescriptorLength: 93
ENDPOINT DESCRIPTOR
    bLength: 7
    bDescriptorType: 0x05 (ENDPOINT)
    bEndpointAddress: 0x81  IN  Endpoint:1
    bmAttributes: 0x03
    wMaxPacketSize: 8
bInterval: 1

SET CONFIGURATION Request
Setup Data
    bmRequestType: 0x00
        0... .... = Direction: Host-to-device
        .00. .... = Type: Standard (0x0)
        ...0 0000 = Recipient: Device (0x00)
    bRequest: SET CONFIGURATION (9)
    bConfigurationValue: 1
    wIndex: 0 (0x0000)
wLength: 0

SET CONFIGURATION Response
空

SET_IDLE Request
Setup Data
    bmRequestType: 0x21
        0... .... = Direction: Host-to-device
        .01. .... = Type: Class (0x1)
        ...0 0001 = Recipient: Interface (0x01)
    bRequest: SET_IDLE (0x0a)
    wValue: 0x0000
    wIndex: 0
    wLength: 0

SET_IDLE Response
空

GET DESCRIPTOR Request HID Report
Setup Data
    bmRequestType: 0x81
        1... .... = Direction: Device-to-host
        .00. .... = Type: Standard (0x0)
        ...0 0001 = Recipient: Interface (0x01)
    bDescriptorIndex: 0x00
    bDescriptorType: HID Report (0x22)
    wInterfaceNumber: 0
    wDescriptorLength: 157  //93+28=121实际回复长度

GET DESCRIPTOR Response HID Report
HID Report
    Global item (Usage)
    Local item (Usage)
    Main item (Collection)
        Header
        Collection type: Application (0x01)
        Local item (Usage)
        Main item (Collection)
        Global item (Usage)
        Local item (Usage)
        Global item (Logical minimum)
        Global item (Logical maximum)
        Global item (Report size)
        Global item (Report count)
        Main item (Feature)
        Main item (End collection)

URB_INTERRUPT in
None

URB_INTERRUPT in
Leftover Capture Data: 0000ff000000ffff

URB_INTERRUPT in
None

URB_INTERRUPT in
Leftover Capture Data: 0000ff000000ffff

URB_INTERRUPT in
None

URB_INTERRUPT in
Leftover Capture Data: 0000000000000000
