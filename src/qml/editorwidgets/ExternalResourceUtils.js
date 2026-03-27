.pragma library

/**
 * Returns the attachment naming expression text for a given layer and field name.
 * Checks for 'QFieldSync/attachment_naming' first, then falls back to
 * the legacy 'QFieldSync/photo_naming' configuration key.
 */
function getAttachmentNaming(layer, fieldName) {
  if (layer && fieldName) {
    var value;
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
 * Returns a sanitized attachment file path based on the evaluated naming
 * expression result. Falls back to a default path with timestamp based on
 * the document viewer type when the expression does not yield a usable suffix.
 *
 * The documentViewer values correspond to ExternalResource.DocumentViewer enum
 * (0 = file, 1 = image, 3 = audio, 4 = video).
 */
function getAttachmentFilePath(evaluatedFilepath, documentViewer, FileUtils) {
  var filepath = FileUtils.sanitizeFilePath(evaluatedFilepath);

  if (FileUtils.fileSuffix(filepath) === '' && !filepath.endsWith("{extension}") && !filepath.endsWith("{filename}")) {
    var nowStr = (new Date()).toISOString().replace(/[^0-9]/g, '');

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
