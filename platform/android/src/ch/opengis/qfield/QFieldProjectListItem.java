package ch.opengis.qfield;

import java.io.File;

public class QFieldProjectListItem
    implements Comparable<QFieldProjectListItem> {

    private File file;
    private String text;
    private int imageId;
    private int type;

    public static final int TYPE_ITEM = 0;
    public static final int TYPE_SEPARATOR = 1;
    public static final int TYPE_ROOT = 2;
    public static final int TYPE_EXTERNAL_FILES = 3;

    public QFieldProjectListItem(File file, String text, int imageId,
                                 int type) {
        this.file = file;
        this.text = text;
        this.imageId = imageId;
        this.type = type;
    }

    public File getFile() {
        return this.file;
    }

    public String getText() {
        return this.text;
    }

    public int getImageId() {
        return this.imageId;
    }

    public int getType() {
        return this.type;
    }

    public boolean isDirectory() {
        return this.imageId == R.drawable.directory;
    }

    @Override
    public int compareTo(QFieldProjectListItem item) {
        if (this.isDirectory() != item.isDirectory()) {
            return this.isDirectory() ? -1 : 1;
        }
        return this.file.getName().compareToIgnoreCase(
            item.getFile().getName());
    }
}
