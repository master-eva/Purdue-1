import java.util.*;

class Command {
    //fields to represents different kind of command
    int commandType;
    String args[];

    // pre-defined command type
    public static final int COMMAND_ADD = 11;
    public static final int COMMAND_UPDATE = 12;
    public static final int COMMAND_POP = 13;
    public static final int COMMAND_DELETE = 14;
    public static final int COMMAND_END = 15;

    public String[] getArgs() {
        return args;
    }

    //constructor which throws customized exception that can be printed in main
    public Command(String line) {
        args = line.split(" ");
        //deal with input with no space
        if (args.length < 2) args = new String[]{"", ""};
        switch (args[1]) {
            case "T3":
                this.commandType = COMMAND_POP;
                break;
            case "B":
                this.commandType = COMMAND_ADD;
                break;
            case "X":
                this.commandType = COMMAND_DELETE;
                break;
            case "end":
                this.commandType = COMMAND_END;
                break;
            default:
                this.commandType = COMMAND_UPDATE;
                break;
        }
    }

    public int getCommandType() {
        return this.commandType;
    }
}

public class Project4 {
    static Scanner s = new Scanner(System.in);

    // get the input from the user
    // parse the input
    // construct a Command object
    // and return it
    public static Command getNextCommand() {
        if (s.hasNextLine()) {
            String str = s.nextLine().trim();
            if (str.equals("")) return null;
            return new Command(str);
        } else {
            return null;
        }
    }

    public static void main(String[] args) {
        SongCollection songCollection = new SongCollection();
        int line = 0;
        int lastDate = 0;
        Command command;

        try {
            command = getNextCommand();
            while (null != command) {
                line++;
                int date = Integer.parseInt(command.getArgs()[0]);
                if (date <= lastDate) throw new SemanticError(date);
                if (line > 5 && songCollection.getLast() < 3)
                    throw new Exception("Error min size violated");
                lastDate = date;

                switch (command.getCommandType()) {
                    case Command.COMMAND_ADD:
                        if (command.getArgs().length != 2)
                            throw new SyntaxError(line);
                        if (songCollection.getLast() >= 500)
                            throw new Exception("Error max size exceeded date " + date);
                        songCollection.addSong(date);
                        break;
                    case Command.COMMAND_UPDATE:
                        if (command.getArgs().length != 4)
                            throw new SyntaxError(line);
                        if (command.getArgs()[1].charAt(0) != 'S') throw new SyntaxError(line);
                        Song song = songCollection.getSong(Integer.parseInt(command.getArgs()[1].substring(1)));
                        if (song == null)
                            throw new SemanticError(date);
                        songCollection.updateSong(command.getArgs()[1],
                                Integer.parseInt(command.getArgs()[2]), Integer.parseInt(command.getArgs()[3]));
                        break;
                    case Command.COMMAND_POP:
                        if (command.getArgs().length != 2)
                            throw new SyntaxError(line);
                        if (songCollection.getLast() < 3)
                            throw new SemanticError(date);
                        List<Song> pop = songCollection.popular();
                        for (int i = 0; i < 3; i++) {
                            System.out.printf("%d: %s, pop=%d\n",
                                    i + 1, pop.get(i).getName(), pop.get(i).getPopularity());
                        }
                        break;
                    case Command.COMMAND_DELETE:
                        if (command.getArgs().length != 3)
                            throw new SyntaxError(line);
                        if (line > 5 && songCollection.getLast() - Integer.parseInt(command.getArgs()[2]) < 3) {
                            songCollection.deleteSong(songCollection.getLast() - 3);
                            throw new Exception("Error min size violated");
                        }
                        if (songCollection.getLast() - Integer.parseInt(command.getArgs()[2]) < 0)
                            throw new SemanticError(date);
                        songCollection.deleteSong(Integer.parseInt(command.getArgs()[2]));
                        break;
                    case Command.COMMAND_END:
                        if (command.getArgs().length != 2)
                            throw new SyntaxError(line);
                        if (songCollection.getLast() < 2)
                            throw new SemanticError(date);
                        List<Integer> end = songCollection.minMax();
                        System.out.printf("min %d, max %d\n", end.get(1), end.get(0));
                        return;
                    default:
                        throw new SyntaxError(line);
                }
                command = getNextCommand();
            }
        } catch (NumberFormatException e) {
            //e.printStackTrace();
            System.out.println(new SyntaxError(line).getMessage());
        } catch (SyntaxError|SemanticError e) {
            //e.printStackTrace();
            System.out.println(e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
