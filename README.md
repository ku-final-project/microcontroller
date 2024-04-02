# Access Control Terminal Hardware

## Table Of Contents
- [Prerequisites](#prerequisites)
- [How to Setup](#how-to-setupj)


## Prerequisites
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE for VSCode](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)


## How to Setup

### Build ชุดโปรแกรม
1. เมื่อติดตั้ง `Visual Studio Code` และ `PlatformIO IDE for VSCode` แล้วนั้น ให้ทดลอง compile ชุดโปรแกรมด้วยปุ่ม `build` () ด้านล่างของโปรแกรม

![build](asset/1.png)

หากชุดโปรแกรมสามารถ build ได้เรียบร้อย ควรจะเห็นสถานะ `SUCCESS` ขึ้นตามรูปด้านบน

### การตั้งค่า Parameter เบื้องต้น
1. ทำการสร้าง Directory `data` ไว้ที่ Directory นอกสุด
```
mkdir data
cd data
```
2. ทำการสร้างไฟล์