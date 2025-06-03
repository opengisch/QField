#include "catch2.h"
#include "positioning/vendorcommandhelper.h"
#include <QBuffer>

TEST_CASE("VendorCommandHelper", "[bluetooth]")
{
  QBuffer buffer;
  buffer.open(QIODevice::ReadWrite);
  VendorCommandHelper helper(&buffer);

  REQUIRE(helper.configInsDisable());
  REQUIRE(buffer.buffer() == QByteArray("CONFIG INS DISABLE\r\n"));
  buffer.buffer().clear();

  REQUIRE(helper.configImuToAntOffset(1,2,3,0,0,0));
  REQUIRE(buffer.buffer().startsWith("CONFIG IMUTOANT OFFSET 1 2 3"));
}
