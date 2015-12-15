import java.util.*;

public class Song {
    String name;
    int times;
    int likes;
    int popularity;

    public Song(String name) {
        this.name = name;
        this.times = 20;
        this.likes = 20;
        this.popularity = this.times + 2 * this.likes;
    }

    public String getName() {
        return name;
    }

    public int getTimes() {
        return times;
    }

    public int getLikes() {
        return likes;
    }

    //compare this item and param 1, if this is less popular, return true
    public boolean cmp(Song song) {
        if (this.getPopularity() < song.getPopularity())
            return true;
        if (this.getPopularity() > song.getPopularity())
            return false;
        int x = Integer.parseInt(this.getName().substring(1));
        int y = Integer.parseInt(song.getName().substring(1));
        return x < y;
    }

    public int getPopularity() {
        return popularity;
    }

    public int updateSong(int deltaN, int deltaL) {
        this.times += deltaN;
        this.likes += deltaL;
        this.popularity = this.times + 2 * this.likes;
        return deltaN + 2 * deltaL;
    }

    static class SongComparator implements Comparator<Song> {
        @Override
        public int compare(Song song1, Song song2) {
            if (song1.getPopularity() > song2.getPopularity()) return -1;
            if (song1.getPopularity() < song2.getPopularity()) return 1;
            int x = Integer.parseInt(song1.getName().substring(1));
            int y = Integer.parseInt(song2.getName().substring(1));
            return y - x;
        }
    }
}

