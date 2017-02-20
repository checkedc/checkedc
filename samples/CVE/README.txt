CVE ( Common Vulnerabilities and Exposures ) Example

Select Buffer Overflow Cases

1. CVE-2015-0569
   Heap-based   buffer overflow in the private wireless extensions IOCTL implementation in   wlan_hdd_wext.c in the WLAN (aka Wi-Fi) driver for the 
   Linux kernel 3.x and   4.x, as used in Qualcomm Innovation Center (QuIC) Android contributions for   MSM devices and other products, allows 
   attackers to gain privileges via a   crafted application that establishes a packet filter. 
   : https://source.codeaurora.org/quic/la/platform/vendor/qcom-opensource/wlan/qcacld-2.0/commit/?id=a079d716b5481223f0166c644e9ec7c75a31b02c

2. CVE-2016-5443
   drivers/soc/qcom/qdsp6v2/voice_svc.c in the QDSP6v2 Voice Service driver for the Linux kernel 3.x, 
   as used in Qualcomm Innovation Center (QuIC) Android contributions for MSM devices and other prod-
   ucts, allows attackers to cause a denial of service (memory corruption) or possibly have unspecif-
   ied other impact via a write request, as demonstrated by a voice_svc_send_req buffer overflow.
   : https://source.codeaurora.org/quic/la/kernel/msm-3.18/commit/?id=6927e2e0af4dcac357be86ba563c9ae12354bb08
