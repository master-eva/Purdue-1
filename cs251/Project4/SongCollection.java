import java.util.*;
import java.util.List;

public class SongCollection {
    //array to perform a smmh
    Song[] smmh;
    //number of total songs
    int last;
    //use two hash maps to find a song's location in smmh
    HashMap<Integer, Song> dateName;
    HashMap<Song, Integer> nameLocation;

    public SongCollection() {
        smmh = new Song[501];
        last = 0;
        dateName = new HashMap<>();
        nameLocation = new HashMap<>();
    }

    public int getLast() {
        return last;
    }

    public Song getSong(int date) {
        return dateName.get(date);
    }

    // add a song at date, with the name S{date}
    // and default L and N
    public void addSong(int date) throws Exception {
        int current = ++last;
        String name = "S".concat(String.valueOf(date));
        Song song = new Song(name);
        dateName.put(date, song);
        // check if left sibling of node is smaller than itself
        if (current % 2 == 0 && song.cmp(smmh[current - 1])) {
            smmh[current] = smmh[current - 1];
            nameLocation.put(smmh[current], current);
            current--;
        }
        current = up(current, song);
        // have the song settle down in the good position
        smmh[current] = song;
        nameLocation.put(song, current);
        System.out.printf("%s: N=%d, L=%d, pop=%d\n",
                song.getName(), song.getTimes(), song.getLikes(), song.getPopularity());
    }

    int up(int current, Song song) {
        boolean flag = false;
        while (!flag && current > 2) {
            int gp = (current + 1) / 4 - 1;
            int lc = 2 * gp + 1;
            int rc = lc + 1;
            if (song.cmp(smmh[lc])) {
                smmh[current] = smmh[lc];
                nameLocation.put(smmh[lc], current);
                current = lc;
            } else if (!song.cmp(smmh[rc])) {
                smmh[current] = smmh[rc];
                nameLocation.put(smmh[rc], current);
                current = rc;
            } else {
                flag = true;
            }
        }
        return current;
    }

    // a recursive down method in the min heap to reduce the length of code
    void downMin(int n) {
        int plan = n * 2 + 1;
        if (plan + 2 <= last && smmh[plan + 2].cmp(smmh[plan]))
            plan = plan + 2;
        if (plan <= last && smmh[plan].cmp(smmh[n])) {
            Song temp = smmh[n];
            smmh[n] = smmh[plan];
            smmh[plan] = temp;
            nameLocation.put(smmh[n], n);
            nameLocation.put(smmh[plan], plan);
            downMin(plan);
        }
    }

    // a recursive down method in the max heap to reduce the length of code
    void downMax(int n) {
        int plan = n * 2;
        if (plan + 2 <= last && !smmh[plan + 2].cmp(smmh[plan]))
            plan = plan + 2;
        if (plan <= last && !smmh[plan].cmp(smmh[n])) {
            Song temp = smmh[n];
            smmh[n] = smmh[plan];
            smmh[plan] = temp;
            nameLocation.put(smmh[n], n);
            nameLocation.put(smmh[plan], plan);
            downMax(plan);
        }
    }

    // delete the n songs of lowest priorities
    public void deleteSong(int n) {
        //delete min for n times and print before delete
        for (int i = 0; i < n; i++) {
            System.out.printf("%s deleted, pop=%d\n", smmh[1].getName(), smmh[1].getPopularity());
            nameLocation.remove(smmh[1]);
            dateName.remove(Integer.parseInt(smmh[1].getName().substring(1)));
            smmh[1] = smmh[last];
            nameLocation.put(smmh[1], 1);
            smmh[last--] = null;
            if (last == 0) return;
            Song song = smmh[1];
            downMin(1);
            int current = nameLocation.get(song);
            if (current * 2 + 1 > last) {
                if (current + 1 <= last && !song.cmp(smmh[current + 1])) {
                    smmh[current] = smmh[current + 1];
                    nameLocation.put(smmh[current], current);
                    current++;
                }
                current = up(current, song);
                smmh[current] = song;
                nameLocation.put(song, current);
            }
        }
    }

    // update song named by songName
    public void updateSong(String songName, int deltaN, int deltaL) {
        int date = Integer.parseInt(songName.substring(1));
        Song song = dateName.get(date);
        int deltaP = song.updateSong(deltaN, deltaL);
        int current = nameLocation.get(song);
        if (current % 2 == 0) {
            //it is in a max heap
            if (deltaP > 0) {
                // increasing -> goes up
                current = up(current, song);
                smmh[current] = song;
                nameLocation.put(song, current);
            } else if (deltaP < 0) {
                // decreasing -> goes down
                downMax(current);
                current = nameLocation.get(song);
                if (current * 2 + 1 > last) {
                    //it goes to the last line, goes up then
                    if (song.cmp(smmh[current - 1])) {
                        smmh[current] = smmh[current - 1];
                        nameLocation.put(smmh[current], current);
                        current--;
                    }
                    // goes up
                    current = up(current, song);
                    smmh[current] = song;
                    nameLocation.put(song, current);
                }
            }
        } else {
            //it is in a min heap
            if (deltaP < 0) {
                //decreasing -> goes up
                current = up(current, song);
                smmh[current] = song;
                nameLocation.put(song, current);
            } else if (deltaP > 0) {
                //increasing -> goes down
                downMin(current);
                current = nameLocation.get(song);
                if (current * 2 + 1 > last) {
                    //it goes to the last line, goes up then
                    //check if meet the sibling rule
                    if (current + 1 <= last && !song.cmp(smmh[current + 1])) {
                        smmh[current] = smmh[current + 1];
                        nameLocation.put(smmh[current], current);
                        current++;
                    }
                    // goes up
                    current = up(current, song);
                    smmh[current] = song;
                    nameLocation.put(song, current);
                }
            }
        }
        System.out.printf("%s: N=%d, L=%d, pop=%d\n",
                song.getName(), song.getTimes(), song.getLikes(), song.getPopularity());
    }

    // return the 3 most popular songs in the return value
    // with the most popular at index 0
    //      the second popular at index 1
    //      the third popular at index 2
    public List<Song> popular() {
        //list of possible max songs(when n > 14, it's O(1))
        List<Song> candidate = new ArrayList<>();
        for (int i = 1; i < 15; i++) {
            if (smmh[i] == null) break;
            candidate.add(smmh[i]);
        }
        //sort the list
        Collections.sort(candidate, new Song.SongComparator());

        //get the most popular songs
        List<Song> songs = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            songs.add(candidate.get(i));
        }

        return songs;
    }

    // return the popularity
    // of the most and least popular songs
    // the 0th integer in the returned object
    // should be the popularity of the most popular song
    // and the 1st integer should be the popularity of
    // the least popular song
    public List<Integer> minMax() {
        //get the max and min from smmh
        List<Integer> songs = new ArrayList<>();
        songs.add(smmh[2].getPopularity());
        songs.add(smmh[1].getPopularity());
        return songs;
    }
}