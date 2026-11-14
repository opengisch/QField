.pragma library

/**
 * Returns the attachment naming expression text for a given layer and field name.
 * Checks for 'QFieldSync/attachment_naming' first, then falls back to
 * the legacy 'QFieldSync/photo_naming' configuration key.
 */
function getAttachmentNaming(layer, fieldName) {
  if (layer && fieldName) {
    let value;
    if (layer.customProperty('QFieldSync/attachment_naming') !== undefined) {
      value = JSON.parse(layer.customProperty('QFieldSync/attachment_naming'))[fieldName];
      return value !== undefined ? value : '';
    } else if (layer.customProperty('QFieldSync/photo_naming') !== undefined) {
      value = JSON.parse(layer.customProperty('QFieldSync/photo_naming'))[fieldName];
      return value !== undefined ? value : '';
    }
  }
  return '';
}

/**
 * Returns the list of accepted attachment types ('image', 'video', and/or 'audio')
 * for a given layer and field name, as configured in QFieldSync through the
 * 'QFieldSync/attachment_accepted_types' configuration key. An empty list means
 * any attachment type is accepted.
 */
function getAttachmentAcceptedTypes(layer, fieldName) {
  if (layer && fieldName && layer.customProperty('QFieldSync/attachment_accepted_types') !== undefined) {
    const value = JSON.parse(layer.customProperty('QFieldSync/attachment_accepted_types'))[fieldName];
    if (Array.isArray(value)) {
      return value.filter(type => ['image', 'video', 'audio'].includes(type));
    }
  }
  return [];
}

/**
 * Returns whether a given attachment type ('image', 'video', or 'audio') is
 * allowed by a list of accepted attachment types.
 */
function acceptsAttachmentType(acceptedTypes, type) {
  return acceptedTypes.length === 0 || acceptedTypes.includes(type);
}

/**
 * Returns the attachment type ('image', 'video', or 'audio') capture actions
 * should use for a list of accepted attachment types, with images taking
 * priority over videos, and videos over audio clips. Returns an empty string
 * when the list is empty.
 */
function getAttachmentCaptureType(acceptedTypes) {
  for (const type of ['image', 'video', 'audio']) {
    if (acceptedTypes.includes(type)) {
      return type;
    }
  }
  return '';
}

/**
 * Returns a sanitized attachment file path based on the evaluated naming
 * expression result. Falls back to a default path with timestamp based on
 * the document viewer type when the expression does not yield a usable suffix.
 *
 * The documentViewer values correspond to ExternalResource.DocumentViewer enum
 * (0 = file, 1 = image, 3 = audio, 4 = video).
 */
function getAttachmentFilePath(evaluatedFilepath, documentViewer, QfFileUtils) {
  let filepath = QfFileUtils.sanitizeFilePath(evaluatedFilepath);

  if (QfFileUtils.fileSuffix(filepath) === '' && !filepath.endsWith("{extension}") && !filepath.endsWith("{filename}")) {
    let nowStr = (new Date()).toISOString().replace(/[^0-9]/g, '');

    if (documentViewer === 1) {
      // DocumentImage
      filepath = 'DCIM/JPEG_' + nowStr + '.{extension}';
    } else if (documentViewer === 3) {
      // DocumentAudio
      filepath = 'audio/AUDIO_' + nowStr + '.{extension}';
    } else if (documentViewer === 4) {
      // DocumentVideo
      filepath = 'video/VIDEO_' + nowStr + '.{extension}';
    } else {
      // DocumentFile (0) or unknown
      filepath = 'files/' + nowStr + '_{filename}';
    }
  }

  filepath = filepath.replace('\\', '/');
  return filepath;
}
