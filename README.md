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
The tristate key node and emitted event codes will most likely need to be changed [like I had to do for OnePlus 5/5T devices](https://github.com/sailfishos-oneplus5/triambience/commit/15772a57e8571799939bc1b7a80f7d04beb4771e).
