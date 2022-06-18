# Web GUI Template

## Dependencies

### Manjaro / Arch
```bash
sudo pacman -S gtk4 gtkmm-4.0 webkit2gtk-5.0 rapidjson
```

### Fedora
```bash

```

### Debian / Ubuntu
```bash

```

## Debugging

### GTK
Press Ctrl + Shift + D when the app is focused

### HTML
- Run `WEBKIT_INSPECTOR_SERVER=127.0.0.1:1234 ./main`
- Open `inspector://127.0.0.1:1234` with a webkit enabled browser like *epiphany*