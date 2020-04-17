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
Now you can add a `/etc/dconf/db/vendor.d/triambienced.txt` file in your droid-config sparse files to choose the correct tristate key node when needed without having to fork this project, for example:
```
[apps/onyxtristate]
sliderDevice='/dev/input/event6'
```

The [emitted tristate key event codes](https://github.com/sailfishos-oneplus5/triambience-daemon/commit/15772a57e8571799939bc1b7a80f7d04beb4771e#diff-855c3a38d4fc3858a48957f816ac34dfL115) still have to be changed in source for now; expect this to get easier soon ;)
