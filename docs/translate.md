# Translate a QField plugin

QField plugins can be translated into different languages, using the Qt translation tools. This allows you to provide localized versions of your plugin.

In order to translate strings from your plugin, you need to add the `Translator` pragma on the very top of your QML file:

```qml
pragma Translator: MyPlugin
```

!!! important
    The `pragma Translator: MyPlugin` must be the very-first line of your QML file, before any import statements.

Then use the `qsTranslate` function in your QML code, e.g.:

```qml
Label {
    text: qsTranslate("MyPlugin", "This is a translatable string.")
    wrapMode: Text.WordWrap
    font: Theme.defaultFont
    color: Theme.mainTextColor
}
```

!!! important
    You need to provide a context to the `qsTranslate` function, which is the first passed argument. This context **must** match with the `Translator` pragma key on top of the QML file.

## App-Wide plugin

Here are the steps to generate translation files for your plugin:

!!! info
    The following commands assume you are in the root folder of your plugin, and that your main QML file is called `main.qml`.

- Generate the `ts` files using the `lupdate` tool:

```sh
# e.g. french
lupdate main.qml -ts main_fr.ts
# e.g. spanish
lupdate main.qml -ts main_es.ts
# e.g. german
lupdate main.qml -ts main_de.ts
```

!!! warning
    If you have both Qt5 and Qt6 tools installed, make sure you use the Qt6 ones, e.g. by using `/usr/lib/qt6/bin/lupdate` on Linux.

- Translate the `.ts` files, e.g. using Qt Linguist:

```sh
linguist main_fr.ts
linguist main_es.ts
linguist main_de.ts
```

- Compile the strings into `.qm` files using the `lrelease` tool:

```sh
lrelease main_fr.ts -qm main_fr.qm
lrelease main_es.ts -qm main_es.qm
lrelease main_de.ts -qm main_de.qm
```

!!! warning
    If you have both Qt5 and Qt6 tools installed, make sure you use the Qt6 ones, e.g. by using `/usr/lib/qt6/bin/lrelease` on Linux.

And that's it :rocket:! Now the strings in your QField plugin will be translated into the languages you provided `.qm` files for.
