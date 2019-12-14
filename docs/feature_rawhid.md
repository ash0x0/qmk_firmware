# Raw HID

Raw HID allows for bidirectional communication between keyboard and a host computer over HID interface. It allows for 32 byte bidirectional packet transfer in QMK.

There are two main components to getting raw HID working with your keyboard.

## Keyboard firmware

The implementation is fairly straightforward for the firmware.
In your `rules.mk` add:

```make
RAW_ENABLE = yes
```

In your `keymap.c` include `"raw_hid.h"` and implement the following:

```C
void raw_hid_receive(uint8_t *data, uint8_t length) {
    // Your code goes here. data is the packet received from host.
}
```

The `"raw_hid.h"` header also includes `void raw_hid_send(uint8_t *data, uint8_t length);`
which allows sending packets from keyboard to host. It can be used for debugging when building your host by returning all data back to host to test communication.

```C
void raw_hid_receive(uint8_t *data, uint8_t length) {
    raw_hid_send(data, length);
}
```

All Raw HID packets by default have size `RAW_EPSIZE` which is `32 bytes`, the actual length is determined by `uint8_t length` both when sending and receiving.

Make sure to flash raw enabled firmware before proceeding with working on the host side.

## Host (PC/Mac/Linux)

This is the more complicated part as it will require some digging.

To connect your host computer to your keyboard with raw HID you need four pieces of information about your keyboard:

1. Vendor ID
2. Product ID
3. Usage Page
4. Usage

The first two can easily be found in your keyboard's `config.h` in the keyboard's main directory under `VENDOR_ID` and `PRODUCT_ID`.

The second two are slightly harder and can vary by vendor. Most of the time though for most QMK keyboards **usage page is `0xFF60`** and **usage is `0x0061`**.

#### The harder way

If these values don't work for you, you can easily discover the usage page and usage for your keyboard by finding the values for `tag 4` for usage page and `tag 8` for usage in the hid interface descriptor for your keyboard by using any form of hid dump or any hid library to find the descriptor and extract the values from the tags.
 
The easy way to do this for linux and mac users is to use teensy raw HID [host implementation](https://www.pjrc.com/teensy/rawhid.html "PJRC Teensy") to find the two tags' values. It is also recommended to use this host implementation as a starting point for your own host on any platform.

Download for your proper platform and extract the files. You'll need to go into `hid_<platform>.c` and enable logging by commenting out the first line in this block at the beginning of either file.

```
#define printf(...) // comment this out to get lots of info printed


// a list of all opened HID devices, so the caller can
// simply refer to them by number
typedef struct hid_struct hid_t;
typedef struct buffer_struct buffer_t;
static hid_t *first_hid = NULL;
static hid_t *last_hid = NULL;
```

**TODO:** Find out the easiest way to get the values for these tags on windows.

Then make and run `raw_hidtest.c`. If you are on linux, ensure first you have the proper [udev rule](https://docs.qmk.fm/#/faq_build?id=linux-udev-rules).
You should get output similar to this:

```txt
device: vid=<Vendor ID>, pic=<Product ID>, with # iface
  type 3, 1, 1
  type 3, 1, 2
  type 3, 1, 0
  type 3, 0, 0
    OUT endpoint #
    IN endpoint #
  hid interface (generic)
  descriptor, len=32
  tag: 4, val #
  tag: 8, val #
  type 3, 0, 0
    OUT endpoint #
    IN endpoint #
  hid interface (generic)
  descriptor, len=32
  tag: 4, val <Usage Page>
  tag: 8, val <Usage>
```

Note that we are looking for the final two tag lines, the first two tag lines describe another interface that isn't used for HID. It's easy to figure out the correct tag lines as `tag: 4, val #` which determines the usage page will have an apparent hexadecimal address, usage line is the one immediately after. 

### Building your host

You can build you host using any language that has an avialable HID implementation library if you don't wish to make your own. The ones we know of for popular languages are:

* Node: [node-hid](https://github.com/node-hid/node-hid).
* C: [hidapi](https://github.com/signal11/hidapi).
* Java: [purejavahidapi](https://github.com/nyholku/purejavahidapi) and [hid4java](https://github.com/gary-rowe/hid4java).
* Python: [pyhidapi](https://pypi.org/project/hid/).

This is not an exhaustive cross-platofrm list but should get you started. There are no special requirements for using raw HID so any HID library should work.

Now that you have all four pieces of information required to open HID interface to your keyboard. All you need to do is use your library's available functions to open the device with these parameters.

Continuing with the example teensy host, the particular function provided to open the device is `int rawhid_open(int max, int vid, int pid, int usage_page, int usage)` with `max` being the maximum number of attempts. It should go without saying that regardless of the library you're using, you should always make sure to close the interface when finished. Depending on the operating system and your particular environment there may be issues connecting to it again afterwards with another client or another instance of the same client if it's not explicitly closed. The functions for that in teensy host are `void rawhid_close(int num)` and `static void hid_close(hid_t *hid)` and `static void free_all_hid(void)`. We recommend you close using `hid_close(hid_t *hid)` at the end of your communication and `static void free_all_hid(void)` when client is closed, `rawhid_test.c` doesn't do this by default.

Note that Vendor ID and Product ID are not actually required to open the device. They are used only to filter to a specific device out of the many HID devices you have plugged in. Many library will give you the option to open the device using Product Name or Manufacturer Name instead, `node-hid` being a prime example. This will create issues however for devices with builtin USB Hub or any extra HID interfaces where you will have multiple interfaces with the same name or from the same manufacturer. The Vendor ID together with Product ID create a unique designation to a single interface and will not exhibit this problem. Therefore, even if your library doesn't require you to, it is best to use them to avoid issues.

Unlike Vendor ID and Product ID though, Usage Page and Usage are necessary for successful communication.

For building a better host with added features, check out [VIA](https://github.com/olivia/via-config).
