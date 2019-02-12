package ch.opengis.qfield;

import java.io.File;

public class QFieldProjectListItem implements Comparable<QFieldProjectListItem>{

    private File file;
    private String text;
    private int imageId;

    public QFieldProjectListItem(File file, String text, int imageId){
        this.file = file;
        this.text = text;
        this.imageId = imageId;
    }

    public File getFile(){
        return this.file;
    }

    public String getText(){
        return this.text;
    }

    public int getImageId(){
        return this.imageId;
    }

    @Override
    public int compareTo(QFieldProjectListItem item){
        return this.file.getName().compareTo(item.getFile().getName());
    }
}
