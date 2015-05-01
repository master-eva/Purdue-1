
#include <gtk/gtk.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_RESPONSE 10240

GtkListStore * list_users;
GtkListStore * list_rooms;
GtkWidget * messages_display_view;
GtkWidget * messages_send_view;
GtkWidget *messages;
GtkWidget *myMessage;
GtkWidget *host_widget;
GtkWidget *port_widget;
char * host;
int port;
GtkWidget *welcome_window;
GtkWidget *message_window;
GtkWidget *window;
GtkWidget *table;
GtkWidget *user_list;
GtkWidget *room_list;
GtkWidget *username;
const char * user = NULL;
const char * pw = NULL;
const char * room = NULL;
GtkWidget *password;
GtkWidget *roomname;
GtkWidget *rooms_tree_view;
GtkWidget *users_tree_view;
int nrooms = 0;
int maxrooms = 100;
char* room_array[100];

void hide_message_window(){
    gtk_widget_destroy(message_window);
    gtk_widget_show(window);
}

void create_ok_window(const char * message){
    message_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (message_window), "OK");
    gtk_container_set_border_width (GTK_CONTAINER (message_window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (message_window), 200, 100);
    gtk_widget_show (message_window);

    GtkWidget *table = gtk_table_new (3, 3, TRUE);
    gtk_container_add (GTK_CONTAINER (message_window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (table), 5);
    gtk_widget_show (table);

    //Add message label
    GtkWidget* message_label = gtk_label_new(message);
    gtk_table_attach_defaults (GTK_TABLE (table), message_label, 0, 3, 0, 1);
    gtk_widget_show(message_label);

    GtkWidget *ok_button = gtk_button_new_with_label ("OK");
    gtk_table_attach_defaults(GTK_TABLE (table), ok_button, 2, 3, 2, 3); 
    gtk_widget_show (ok_button);
    g_signal_connect (ok_button, "clicked", G_CALLBACK (hide_message_window), NULL);
    gtk_widget_show(message_window);
}

void create_error_window(const char * message){
    gtk_widget_hide(window);
    message_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (message_window), "ERROR");
    gtk_container_set_border_width (GTK_CONTAINER (message_window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (message_window), 200, 100);
    gtk_widget_show (message_window);

    GtkWidget *table = gtk_table_new (3, 3, TRUE);
    gtk_container_add (GTK_CONTAINER (message_window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (table), 5);
    gtk_widget_show (table);

    //Add message label
    GtkWidget* message_label = gtk_label_new(message);
    gtk_table_attach_defaults (GTK_TABLE (table), message_label, 0, 3, 0, 1);
    gtk_widget_show(message_label);

    GtkWidget *ok_button = gtk_button_new_with_label ("OK");
    gtk_table_attach_defaults(GTK_TABLE (table), ok_button, 2, 3, 2, 3); 
    gtk_widget_show (ok_button);
    g_signal_connect (ok_button, "clicked", G_CALLBACK (hide_message_window), NULL);
    gtk_widget_show(message_window);
}

void hide_welcome_error_window(){
    gtk_widget_hide(message_window);
    gtk_widget_show(welcome_window);
}

void create_welcome_error_window(const char * message){
    message_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (message_window), "ERROR");
    gtk_container_set_border_width (GTK_CONTAINER (message_window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (message_window), 200, 100);
    gtk_widget_show (message_window);

    GtkWidget *table = gtk_table_new (3, 3, TRUE);
    gtk_container_add (GTK_CONTAINER (message_window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (table), 5);
    gtk_widget_show (table);

    //Add message label
    GtkWidget* message_label = gtk_label_new(message);
    gtk_table_attach_defaults (GTK_TABLE (table), message_label, 0, 3, 0, 1);
    gtk_widget_show(message_label);

    GtkWidget *ok_button = gtk_button_new_with_label ("OK");
    gtk_table_attach_defaults(GTK_TABLE (table), ok_button, 2, 3, 2, 3); 
    gtk_widget_show (ok_button);
    g_signal_connect (ok_button, "clicked", G_CALLBACK (hide_welcome_error_window), NULL);
    gtk_widget_show(message_window);
}

void test_client_socket() {
    gtk_widget_hide(welcome_window);

    host = strdup(gtk_entry_get_text(GTK_ENTRY(host_widget)));
    port = atoi(strdup(gtk_entry_get_text(GTK_ENTRY(port_widget))));
    bool flag = true;

    // Initialize socket address structure
    struct  sockaddr_in socketAddress;
    
    // Clear sockaddr structure
    memset((char *)&socketAddress,0,sizeof(socketAddress));
    
    // Set family to Internet 
    socketAddress.sin_family = AF_INET;
    
    // Set port
    socketAddress.sin_port = htons((u_short)port);
    
    // Get host table entry for this host
    struct  hostent  *ptrh = gethostbyname(host);
    if ( ptrh == NULL ) {
        flag = false;
        create_welcome_error_window("Wrong Host.");
        return;
    }
    
    // Copy the host ip address to socket address structure
    memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
    
    // Get TCP transport protocol entry
    struct  protoent *ptrp = getprotobyname("tcp");
    if (ptrp == NULL ) {
        flag = false;
        create_welcome_error_window("Error when doing tcp.");
        return;
    }
    
    // Create a tcp socket
    int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
    if (sock < 0) {
        flag = false;
        create_welcome_error_window("Error when creating socket.");
        return;
    }
    
    // Connect the socket to the specified server
    if (connect(sock, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0) {
        flag = false;
        create_welcome_error_window("Error when connecting.");
        return;
    }
    
    if (flag) {
        gtk_widget_hide(welcome_window);
        gtk_widget_show(window);
    }

    close(sock);
}

int open_client_socket() {
    // Initialize socket address structure
    struct  sockaddr_in socketAddress;
    
    // Clear sockaddr structure
    memset((char *)&socketAddress,0,sizeof(socketAddress));
    
    // Set family to Internet 
    socketAddress.sin_family = AF_INET;
    
    // Set port
    socketAddress.sin_port = htons((u_short)port);
    
    // Get host table entry for this host
    struct  hostent  *ptrh = gethostbyname(host);
    if ( ptrh == NULL ) {
        create_welcome_error_window("Sever Closed, Connect another.");
        return -1;
    }
    
    // Copy the host ip address to socket address structure
    memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
    
    // Get TCP transport protocol entry
    struct  protoent *ptrp = getprotobyname("tcp");
    if ( ptrp == NULL ) {
        create_welcome_error_window("Sever Closed, Connect another.");
        return -1;
    }
    
    // Create a tcp socket
    int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
    if (sock < 0) {
        create_welcome_error_window("Sever Closed, Connect another.");
        return -1;
    }
    
    // Connect the socket to the specified server
    if (connect(sock, (struct sockaddr *)&socketAddress,
            sizeof(socketAddress)) < 0) {
        create_welcome_error_window("Sever Closed, Connect another.");
        return -1;
    }
    
    return sock;
}


/* Create the list of "messages" */
static GtkWidget *create_room_list(GtkListStore *model ) {
    GtkWidget *scrolled_window;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC,  GTK_POLICY_AUTOMATIC);
   
    rooms_tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), rooms_tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (rooms_tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (rooms_tree_view);
   
    cell = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Rooms", cell, "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (rooms_tree_view),
                     GTK_TREE_VIEW_COLUMN (column));

    return scrolled_window;
}

/* Create the list of "messages" */
static GtkWidget *create_user_list(GtkListStore *model ) {
    GtkWidget *scrolled_window;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC,  GTK_POLICY_AUTOMATIC);
   
    users_tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), users_tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (users_tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (users_tree_view);
   
    cell = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Users", cell, "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (users_tree_view),
                     GTK_TREE_VIEW_COLUMN (column));

    return scrolled_window;
}

   
/* Add some text to our text widget - this is a callback that is invoked
when our window is realized. We could also force our window to be
realized with gtk_widget_realize, but it would have to be part of
a hierarchy first */

static void insert_text( GtkTextBuffer *buffer, const char * initialText ) {
   GtkTextIter iter;
 
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter, initialText,-1);
}
   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_message_display( const char * initialText) {
   GtkWidget *scrolled_window;
   GtkTextBuffer *buffer;

   messages_display_view = gtk_text_view_new ();
   gtk_text_view_set_editable (GTK_TEXT_VIEW(messages_display_view), false);
   gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW(messages_display_view), false);
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (messages_display_view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), messages_display_view);
   insert_text (buffer, initialText);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

static GtkWidget *create_message_send( const char * initialText) {
   GtkWidget *scrolled_window;
   GtkTextBuffer *buffer;

   messages_send_view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (messages_send_view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), messages_send_view);
   insert_text (buffer, initialText);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

void send_add_user(){
    int sock = open_client_socket();

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"ADD-USER %s %s", gtk_entry_get_text(GTK_ENTRY(username)), gtk_entry_get_text(GTK_ENTRY(password)));

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

    if (strcmp(response,"OK\r\n") == 0) {
        create_ok_window("OK");
    } else {
        create_error_window(response);
    } 

    close(sock);
}

void room_array_init() {
    nrooms = 0;
    int i;
    for (i = 0; i < maxrooms; i++) {
        room_array[i] = NULL;
    }
}

void add_room_to_array(char * room) {
    room_array[nrooms] = strdup(room);
    nrooms++;
    if (nrooms >= maxrooms) {
        maxrooms += 10;
        int i;
        for (i = nrooms; i < maxrooms; i++) {
            room_array[i] = NULL;
        }
    }
}

void update_list_users(char * response) {
    GtkTreeIter iter;
    int i;

    list_users = gtk_list_store_new (1, G_TYPE_STRING);

    char * word =  (char *)malloc(100 * sizeof(char));
    char * p = word;
    int length = 0;

    while (*response != '\0') {
        if ((*response != '\r') && (*response != '\n')) {
            *p = *response;
            p++;
            response++;
            length ++;
        } else {
            if (length != 0) {
                *p = '\0';
                length = 0;
                gtk_list_store_append (GTK_LIST_STORE (list_users), &iter);
                gtk_list_store_set (GTK_LIST_STORE (list_users), &iter, 0, word, -1); 
                p = word;
            }
            response++;
        }
    }

    if (length != 0) {
        *p = '\0';
        length = 0;
        gtk_list_store_append (GTK_LIST_STORE (list_users), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_users), &iter, 0, word, -1);
        p = word; 
    }

    free(word);
}

void update_list_rooms(char * response) {
    GtkTreeIter iter;
    int i;

    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    room_array_init();

    char * word =  (char *)malloc(100 * sizeof(char));
    char * p = word;
    int length = 0;

    while (*response != '\0') {
        if ((*response != '\r') && (*response != '\n')) {
            *p = *response;
            p++;
            response++;
            length ++;
        } else {
            if (length != 0) {
                *p = '\0';
                length = 0;
                if (strcmp(word,"OK") != 0) {
                    gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
                    gtk_list_store_set (GTK_LIST_STORE (list_rooms), &iter, 0, word, -1); 
                    add_room_to_array(word);
                }
                p = word;
            }
            response++;
        }
    }

    if (length != 0) {
        *p = '\0';
        length = 0;
        gtk_list_store_append (GTK_LIST_STORE (list_rooms), &iter);
        gtk_list_store_set (GTK_LIST_STORE (list_rooms), &iter, 0, word, -1);
        add_room_to_array(word);
        p = word; 
    }

    free(word);
}

bool get_all_users(){
    int sock = open_client_socket();

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"GET-ALL-USERS %s %s", user, pw);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;
    //printf("%s",response);

    if (strcmp(response,"ERROR (Wrong password)\r\n") == 0) {
        create_error_window("Wrong password");
        return FALSE;
    } 

    update_list_users(response);
    gtk_container_remove(GTK_CONTAINER(table), user_list);
    user_list = create_user_list (list_users);
    gtk_table_attach_defaults (GTK_TABLE (table), user_list, 0, 2, 0, 3);
    gtk_widget_show (user_list);
    return TRUE;
}

void get_all_rooms(){
    int sock = open_client_socket();

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"LIST-ROOMS %s %s", user, pw);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;
    //printf("%s",response);

    update_list_rooms(response);
    gtk_container_remove(GTK_CONTAINER(table), room_list);
    room_list = create_room_list (list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), room_list, 2, 4, 0, 3);
    gtk_widget_show (room_list);
}

void get_users_in_room() {
    int sock = open_client_socket();

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"GET-USERS-IN-ROOM %s %s %s", user, pw, room);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;
    //printf("%s",response);

    if (strcmp(response,"ERROR (Wrong password)\r\n") == 0) {
        create_error_window((response));
        return;
    } 

    update_list_users(response);
    gtk_container_remove(GTK_CONTAINER(table), user_list);
    user_list = create_user_list (list_users);
    gtk_table_attach_defaults (GTK_TABLE (table), user_list, 0, 2, 0, 3);
    gtk_widget_show (user_list);
}

void log_in(){
    user = gtk_entry_get_text(GTK_ENTRY(username));
    pw = gtk_entry_get_text(GTK_ENTRY(password));
    room = NULL;
    if (!get_all_users()) return;
    get_all_rooms();

    gtk_container_remove(GTK_CONTAINER(table), messages);
    messages = create_message_display("Messages will display here when you are in room.");
    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 4, 8);
    gtk_widget_show (messages);
}

void create_room() {
    int sock = open_client_socket();

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"CREATE-ROOM %s %s %s", user, pw, gtk_entry_get_text(GTK_ENTRY(roomname)));

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

    if (strcmp(response,"OK\r\n") == 0) {
        create_ok_window("OK");
    } else {
        create_error_window(response);
    } 

    get_all_rooms();
}

void get_messages() {
    int sock = open_client_socket();

    char * command = (char *)malloc(300 * sizeof(char));
    sprintf(command,"GET-MESSAGES %s %s 0 %s", user, pw, room);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

    gtk_container_remove(GTK_CONTAINER(table), messages);
    messages = create_message_display(response);
    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 4, 8);
    gtk_widget_show (messages);
}

void send_message(){
    int sock = open_client_socket();
    char * command = (char *)malloc(300 * sizeof(char));
    
    GtkTextIter start;
    GtkTextIter end;
    
    gtk_text_buffer_get_start_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(messages_send_view)), &start);
    gtk_text_buffer_get_end_iter(gtk_text_view_get_buffer(GTK_TEXT_VIEW(messages_send_view)), &end);

    char * message = gtk_text_buffer_get_text (gtk_text_view_get_buffer(GTK_TEXT_VIEW(messages_send_view)), &start, &end, FALSE);
    
    sprintf(command,"SEND-MESSAGE %s %s %s %s", user, pw, room, message);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

    if (strcmp(response,"OK\r\n") == 0) {
        create_ok_window("OK");
        get_messages();
    } else {
        create_error_window(response);
    }
}

void enter_room() {
    int sock = open_client_socket();

    GtkTreePath *path;
    GtkTreeViewColumn *column;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(rooms_tree_view), &path, &column);

    if (path == NULL) {
        create_error_window("Choose a room.");
        return;
    }
    int i = atoi(gtk_tree_path_to_string(path));
    room = room_array[i];

    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"ENTER-ROOM %s %s %s", user, pw, room);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

 if (strcmp(response,"OK\r\n") == 0) {
        create_ok_window("OK");
        get_users_in_room();
        get_all_rooms();
        get_messages();

        sock = open_client_socket();
        sprintf(command,"SEND-MESSAGE %s %s %s %s", user, pw, room, "enters room.");

        write(sock, command, strlen(command));
        write(sock, "\r\n",2);
        close(sock);
    } else {
        create_error_window(response);
    } 
}

void leave_room() {
    GtkTreePath *path;
    GtkTreeViewColumn *column;
    gtk_tree_view_get_cursor(GTK_TREE_VIEW(rooms_tree_view), &path, &column);

    if (path == NULL) {
        create_error_window("Choose a room.");
        return;
    }

    int i = atoi(gtk_tree_path_to_string(path));
    room = room_array[i];

    int sock = open_client_socket();
    char * command = (char *)malloc(100 * sizeof(char));
    sprintf(command,"SEND-MESSAGE %s %s %s %s", user, pw, room, "leaves room.");
    
    write(sock, command, strlen(command));
    write(sock, "\r\n",2);
    close(sock);

    sock = open_client_socket();
    sprintf(command,"LEAVE-ROOM %s %s %s", user, pw, room);

    write(sock, command, strlen(command));
    write(sock, "\r\n",2);

    char * response = (char *)malloc(MAX_RESPONSE * sizeof(char));
    int n = 0;
    int len = 0;
    while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
        len += n;
    }
    response[len]=0;

    if (strcmp(response,"OK\r\n") == 0) {
        if (!get_all_users()) return;
        get_all_rooms();
        room = NULL;

        gtk_container_remove(GTK_CONTAINER(table), messages);
        messages = create_message_display("Messages in Room will display here.");
        gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 4, 8);
        gtk_widget_show (messages);

    } else {
        create_error_window(response);
    } 
}

bool do_all() {
    if (room == NULL || user == NULL || pw == NULL) return TRUE;
    get_messages();
    get_users_in_room();
    get_all_rooms();
    return TRUE;
}

int main(int argc, char *argv[]){
    gtk_init (&argc, &argv);

    welcome_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (welcome_window), "IRC Client/Welcome");
    g_signal_connect (welcome_window, "destroy",
                  G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (welcome_window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (welcome_window), 250, 300);

    gtk_widget_show (welcome_window);
    GtkWidget *welcome_table = gtk_table_new (5, 3, TRUE);
    gtk_container_add (GTK_CONTAINER (welcome_window), welcome_table);
    gtk_table_set_row_spacings(GTK_TABLE (welcome_table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (welcome_table), 5);
    gtk_widget_show (welcome_table);

    //Add host entry
    host_widget = gtk_entry_new();
    gtk_entry_set_text (GTK_ENTRY (host_widget), "host");
    gtk_table_attach_defaults (GTK_TABLE (welcome_table), host_widget, 0, 3, 1, 2);
    gtk_widget_show (host_widget);

    //Add port entry
    port_widget = gtk_entry_new();
    gtk_entry_set_text (GTK_ENTRY (port_widget), "port");
    gtk_table_attach_defaults (GTK_TABLE (welcome_table), port_widget, 0, 3, 2, 3);
    gtk_widget_show (port_widget);

    GtkWidget *connect_button = gtk_button_new_with_label ("Connect");
    gtk_table_attach_defaults(GTK_TABLE (welcome_table), connect_button, 2, 3, 4, 5); 
    gtk_widget_show (connect_button);
    g_signal_connect (connect_button, "clicked", G_CALLBACK (test_client_socket), NULL);

   
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "IRC Client");
    g_signal_connect (window, "destroy",
	              G_CALLBACK (gtk_main_quit), NULL);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request (GTK_WIDGET (window), 450, 400);

    // Create a table to place the widgets. Use a 7x4 Grid (7 rows x 4 columns)
    table = gtk_table_new (12, 4, TRUE);
    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_table_set_row_spacings(GTK_TABLE (table), 5);
    gtk_table_set_col_spacings(GTK_TABLE (table), 5);
    gtk_widget_show (table);

    // Add list of users. Use columns 2 to 4 (exclusive) and rows 0 to 4 (exclusive)
    list_users = gtk_list_store_new (1, G_TYPE_STRING);
    user_list = create_user_list (list_users);
    gtk_table_attach_defaults (GTK_TABLE (table), user_list, 0, 2, 0, 3);
    gtk_widget_show (user_list);

    // Add list of rooms. Use columns 2 to 4 (exclusive) and rows 0 to 4 (exclusive)
    list_rooms = gtk_list_store_new (1, G_TYPE_STRING);
    room_list = create_room_list (list_rooms);
    gtk_table_attach_defaults (GTK_TABLE (table), room_list, 2, 4, 0, 3);
    gtk_widget_show (room_list);

    // Add get-users button. Use columns 0 to 1 (exclusive) and rows 7 to 8 (exclusive)
    GtkWidget *get_users_button = gtk_button_new_with_label ("Get All Users");
    gtk_table_attach_defaults(GTK_TABLE (table), get_users_button, 1, 2, 3, 4); 
    gtk_widget_show (get_users_button);
    g_signal_connect (get_users_button, "clicked", G_CALLBACK (get_all_users), NULL);

    
    // Add get-users button. Use columns 0 to 1 (exclusive) and rows 7 to 8 (exclusive)
    GtkWidget *enter_room_button = gtk_button_new_with_label ("Enter Room");
    gtk_table_attach_defaults(GTK_TABLE (table), enter_room_button, 2, 3, 3, 4); 
    gtk_widget_show (enter_room_button);
    g_signal_connect (enter_room_button, "clicked", G_CALLBACK (enter_room), NULL);


    // Add get-users button. Use columns 0 to 1 (exclusive) and rows 7 to 8 (exclusive)
    GtkWidget *leave_room_button = gtk_button_new_with_label ("Leave Room");
    gtk_table_attach_defaults(GTK_TABLE (table), leave_room_button, 3, 4, 3, 4); 
    gtk_widget_show (leave_room_button);
    g_signal_connect (leave_room_button, "clicked", G_CALLBACK (leave_room), NULL);
   
    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 
    messages = create_message_display ("Messages in Room will display here.");
    gtk_table_attach_defaults (GTK_TABLE (table), messages, 0, 4, 4, 8);
    gtk_widget_show (messages);

    // Add messages text. Use columns 0 to 4 (exclusive) and rows 4 to 7 (exclusive) 
    myMessage = create_message_send ("Hello!");
    gtk_table_attach_defaults (GTK_TABLE (table), myMessage, 0, 4, 8, 10);
    gtk_widget_show (myMessage);

    //Add user name entry
    username = gtk_entry_new();
    gtk_entry_set_text (GTK_ENTRY (username), "username");
    gtk_table_attach_defaults (GTK_TABLE (table), username, 0, 1, 10, 11);
    gtk_widget_show (username);

    //Add password entry
    password = gtk_entry_new();
    gtk_entry_set_text (GTK_ENTRY (password), "password");
    gtk_entry_set_visibility (GTK_ENTRY (password), FALSE);
    gtk_table_attach_defaults (GTK_TABLE (table), password, 0, 1, 11, 12);
    gtk_widget_show (password);

    // Add sign up button.
    GtkWidget *create_user_button = gtk_button_new_with_label ("Create User");
    gtk_table_attach_defaults(GTK_TABLE (table), create_user_button, 1, 2, 10, 11); 
    gtk_widget_show (create_user_button);
    g_signal_connect (create_user_button, "clicked", G_CALLBACK (send_add_user), NULL);


    // Add log in button.
    GtkWidget *log_in_button = gtk_button_new_with_label ("Log in");
    gtk_table_attach_defaults(GTK_TABLE (table), log_in_button, 1, 2, 11, 12); 
    gtk_widget_show (log_in_button);
    g_signal_connect (log_in_button, "clicked", G_CALLBACK (log_in), NULL);
    
    //Add roomname entry
    roomname = gtk_entry_new();
    gtk_entry_set_text (GTK_ENTRY (roomname), "roomname");
    gtk_table_attach_defaults (GTK_TABLE (table), roomname, 2, 3, 10, 11);
    gtk_widget_show (roomname);

    // Add create room button.
    GtkWidget *create_room_button = gtk_button_new_with_label ("Create Room");
    gtk_table_attach_defaults(GTK_TABLE (table), create_room_button, 2, 3, 11, 12); 
    gtk_widget_show (create_room_button);
    g_signal_connect (create_room_button, "clicked", G_CALLBACK (create_room), NULL);
  


    // Add send button. 
    GtkWidget *send_button = gtk_button_new_with_label ("Send");
    gtk_table_attach_defaults(GTK_TABLE (table), send_button, 3, 4, 10, 12); 
    gtk_widget_show (send_button);  
    g_signal_connect (send_button, "clicked", G_CALLBACK (send_message), NULL);

    g_timeout_add(5000, (GSourceFunc) do_all, (gpointer) window);

    gtk_main ();

    return 0;
}

