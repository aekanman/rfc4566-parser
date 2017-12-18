# rfc4566 Parser

Sample application to parse Session Description Protocol(SDP) with RFC4566 standards.

Example input:
```
v=0
o=- 1445342398 1445342398 IN IP4 172.29.80.65
s=NMOS Stream
t=0 0
m=video 5000 RTP/AVP 96
c=IN IP4 232.58.66.134/32
a=source-filter:incl IN IP4 232.58.66.134 172.29.80.65
a=rtpmap:96 raw/90000
a=fmtp:96 sampling=YCbCr-4:2:2; width=1920; height=1080; depth=10; colorimetry=BT709-2; interlace=1
a=mediaclk:direct=744520299 rate=90000
a=ts-refclk:ptp=IEEE1588-2008:ec-46-70-ff-fe-00-42-c4
```

Output:
```
(payload *) data = 0x0000000100606a20
(int) version = 0
(origin) orig = (username = "-", sess_id = 1445342398, sess_ver = 1445342398, nettype = "IN", addrtype = "IP4", unicast_addr = "172.29.80.65")
(char *) sess_name = 0x00007fff5fbff4b4 "NMOS Stream"
(timing) time = (start = 0, stop = 0)
(media) media = (media = "video", port = 5000, proto = "RTP/AVP", fmt = 96)
(connection) conn = (nettype = "IN", addrtype = "IP4", address = "232.58.66.134/32")
(attributes) att = {
  sf = (unknown_str = "incl", nettype = "IN", addrtype = "IP4", address = "232.58.66.134", unicast_addr = "172.29.80.65")
  rtp = (payload_type = 96, encoding_name = "raw", clc_rate = 90000)
  fmtp = (format = 96, sampling = "YCbCr-4:2:2", width = 1920, height = 1080, depth = 10, colorimetry = "BT709-2", interlace = 1)
  mclk = (direct = 744520299, rate = 90000)
  rclk = (ptp = "IEEE1588-2008:ec-46-70-ff-fe-00-42-c4")
}
```

<img src="/Screen Shot.png" alt="Original" height="640" />

## License
MIT © [Atakan Efe Kanman](https://atakanefekanman.com)
