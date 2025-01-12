#!/bin/bash
set -euo pipefail

# Required environment variables:
# APP_PATH: Path to your .app bundle
# DMG_PATH: Path to your .dmg file
# P12_PATH: Path to your .p12 certificate file
# P12_PASSWORD: Password for the .p12 file
# API_KEY_ID: App Store Connect API Key ID
# API_KEY_ISSUER_ID: App Store Connect API Key Issuer ID
# API_KEY_PATH: Path to the API key file (.p8)

if [[ -n ${API_KEY_ID:-} ]]; then
	KEYCHAIN_NAME="notary-keychain"
	KEYCHAIN_PASSWORD="temp-password"
	security create-keychain -p "$KEYCHAIN_PASSWORD" "$KEYCHAIN_NAME"
	security unlock-keychain -p "$KEYCHAIN_PASSWORD" "$KEYCHAIN_NAME"
	security import "$P12_PATH" -k "$KEYCHAIN_NAME" -P "$P12_PASSWORD" -T /usr/bin/codesign
	security set-keychain-settings -t 3600 -l "$KEYCHAIN_NAME"
	security list-keychains -d user -s "$KEYCHAIN_NAME" $(security list-keychains -d user | tr -d '"')
	security set-key-partition-list -S apple-tool:,apple:,codesign: -s -k "$KEYCHAIN_PASSWORD" "$KEYCHAIN_NAME"

	# Sign the .app bundle
	codesign --force --options runtime --sign "Developer ID Application" --deep --keychain "$KEYCHAIN_NAME" "$APP_PATH"
fi

create-dmg --volname "QField Installer" \
	--hide-extension "qfield.app" \
	--volicon "./platform/macos/installer.icns" \
	--background "./platform/macos/installer_background.png" \
	--window-pos 200 120 \
	--window-size 512 320 \
	--icon-size 100 \
	--icon "qfield.app" 130 160 \
	--app-drop-link 400 155 \
	"$DMG_PATH" "$APP_PATH"

if [[ -n ${API_KEY_ID:-} ]]; then
	codesign --force --options runtime --sign "Developer ID Application" --keychain "$KEYCHAIN_NAME" "$DMG_PATH"

	echo "Submitting for notarization..."
	xcrun notarytool submit "$DMG_PATH" \
		--key "$API_KEY_PATH" \
		--key-id "$API_KEY_ID" \
		--issuer "$API_KEY_ISSUER_ID" \
		--wait

	# Staple the notarization ticket
	xcrun stapler staple "$DMG_PATH"

	# Clean up keychain
	security delete-keychain "$KEYCHAIN_NAME"

	echo "Notarization and stapling complete!"
fi
