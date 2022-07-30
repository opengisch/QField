/***************************************************************************
 barcodevideofilter.cpp - BarcodeVideoFilter

 ---------------------
 begin                : 22.07.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "barcodevideofilter.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>

class BarcodeDecoderThread : public QThread
{
  public:
    explicit BarcodeDecoderThread( BarcodeDecoder *decoder, const QImage &image )
      : QThread()
      , mDecoder( decoder )
      , mImage( image )
    {
    }

  private:
    void run() override
    {
      if ( mDecoder )
      {
        mDecoder->decodeImage( mImage );
      }
    }

    BarcodeDecoder *mDecoder = nullptr;
    QImage mImage;
};

class BarcodeVideoFilterRunnable : public QVideoFilterRunnable
{
  public:
    explicit BarcodeVideoFilterRunnable( BarcodeVideoFilter *filter = nullptr )
      : mFilter( filter )
    {
    }

    QVideoFrame run( QVideoFrame *input,
                     const QVideoSurfaceFormat &surfaceFormat,
                     QVideoFilterRunnable::RunFlags flags ) override
    {
      Q_UNUSED( surfaceFormat );
      Q_UNUSED( flags );

      if ( !mFilter->isDecoding() )
      {
        mFilter->decodeVideoFrame( input );
      }

      return *input;
    }

  private:
    BarcodeVideoFilter *mFilter;
};

BarcodeVideoFilter::BarcodeVideoFilter()
{
}

void BarcodeVideoFilter::setDecoder( BarcodeDecoder *decoder )
{
  if ( mDecoder == decoder )
  {
    return;
  }

  mDecoder = decoder;

  emit decoderChanged();
}

void BarcodeVideoFilter::decodeVideoFrame( QVideoFrame *input )
{
  if ( !mDecoder || isDecoding() )
  {
    return;
  }

#if QT_VERSION >= QT_VERSION_CHECK( 5, 15, 0 )
  QImage image = input->image();
#else
  QImage image;
  if ( input->handleType() == QAbstractVideoBuffer::NoHandle )
  {
    const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat( input->pixelFormat() );
    image = QImage( input->bits(),
                    input->width(),
                    input->height(),
                    input->bytesPerLine(),
                    imageFormat );
  }
  else if ( input->handleType() == QAbstractVideoBuffer::GLTextureHandle )
  {
    // Code taken from https://stackoverflow.com/questions/27829830/convert-qvideoframe-to-qimage
    QImage img( input->width(), input->height(), QImage::Format_ARGB32 );
    GLuint textureId = static_cast<GLuint>( input->handle().toInt() );
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QOpenGLFunctions *f = ctx->functions();
    GLuint fbo;
    f->glGenFramebuffers( 1, &fbo );
    GLint prevFbo;
    f->glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFbo );
    f->glBindFramebuffer( GL_FRAMEBUFFER, fbo );
    f->glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0 );
    f->glReadPixels( 0, 0, input->width(), input->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits() );
    f->glBindFramebuffer( GL_FRAMEBUFFER, static_cast<GLuint>( prevFbo ) );
    image = img.rgbSwapped();
  }
#endif

  if ( !image.isNull() && image.format() != QImage::Format_ARGB32 )
  {
    image = image.convertToFormat( QImage::Format_ARGB32 );
  }

  mDecodingThread = new BarcodeDecoderThread( mDecoder, image );
  connect( mDecodingThread, &QThread::finished, this, [=] {
    mDecodingThread->deleteLater();
    mDecodingThread = nullptr;
  } );
  mDecodingThread->start();
}

QVideoFilterRunnable *BarcodeVideoFilter::createFilterRunnable()
{
  return new BarcodeVideoFilterRunnable( this );
}
