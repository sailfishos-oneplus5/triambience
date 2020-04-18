# triambience-daemon
Control active ambiences using a tristate "notification" slider under SFOS.

## Configuration
Chosen ambiences are controlled via dconf @ `/apps/onyxtristate/`:
```
[apps/onyxtristate]
top='silent'
middle='sailing'
bottom='origami'
```

There is also a [Sailfish OS settings plugin](https://github.com/sailfishos-oneplus5/onyx-triambience-settings-plugin) available for simple control of these values through the settings app.

## Adaptation
You can now add a `/etc/dconf/db/vendor.d/triambience-daemon.txt` file in your droid-config sparse files to choose the correct tristate key node and emitted key event codes if needed without having to fork this project, for example:
```
[apps/onyxtristate]
sliderDevice='/dev/input/event6'
sliderCodes=[0x25b, 0x25a, 0x259]
```

To find the device event node you can install the `mce-tools` package and run `evdev_trace`; it should be pretty obvious looking at the names which one is your device's tri-state key / notification slider:
```
[nemo@OnePlus5 ~]$ evdev_trace
...
----====( /dev/input/event3 )====----
Name: "tri-state-key"
ID: bus 0x0, vendor, 0x0, product 0x0, version 0x0
Type 0x00 (EV_SYN)
Type 0x01 (EV_KEY)
	 KEY_0258 KEY_0259 KEY_025a KEY_025b
...
```
In this case `/dev/input/event3` is the device node and you can define it as the `sliderDevice` dconf item value and use the number at the end of the device node in the command below.

Now move the tri-state key to it's middle position, run the command below and go through all the positions going top-to-bottom:
```
[nemo@OnePlus5 ~]$ evdev_trace -t 3 | grep '1$'
/dev/input/event3: 1587214927.004 - 0x01/EV_KEY - 0x259/KEY_0259 - 1
/dev/input/event3: 1587214928.329 - 0x01/EV_KEY - 0x25a/KEY_025a - 1
/dev/input/event3: 1587214929.648 - 0x01/EV_KEY - 0x25b/KEY_025b - 1
```
From this example output we can determine that the key top position yields code `0x259`, middle `0x25a` and bottom `0x25b`, which you can place all in the same order as the `sliderCodes` dconf item value.
