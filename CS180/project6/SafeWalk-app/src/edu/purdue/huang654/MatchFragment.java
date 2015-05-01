package edu.purdue.huang654;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;

import android.app.Fragment;
import android.util.Log;
import android.view.View.OnClickListener;
import android.os.Bundle;
import android.os.AsyncTask;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * This fragment is the "page" where the application display the log from the
 * server and wait for a match.
 *
 * @author YL
 */
public class MatchFragment extends Fragment implements OnClickListener {
	
	private static final String DEBUG_TAG = "DEBUG";
	
	/**
	 * Activity which have to receive callbacks.
	 */
	private StartOverCallbackListener activity;
	
	/**
	 * AsyncTask sending the request to the server.
	 */
	private Client client;
	
	/**
	 * Coordinate of the server.
	 */
	private String host;
	private int port;
	
	/**
	 * Command the user should send.
	 */
	private String command;
	
	/*
	 * Layout.
	 */
	private TextView status;
	
	/**
	 * Socket.
	 */
	private Socket socket;
	private boolean created = false;
	
	// Class methods
	/**
	 * Creates a MatchFragment
	 * 
	 * @param activity
	 *            activity to notify once the user click on the start over
	 *            Button.
	 * @param host
	 *            address or IP address of the server.
	 * @param port
	 *            port number.
	 * 
	 * @param command
	 *            command you have to send to the server.
	 * 
	 * @return the fragment initialized.
	 */
	// TODO: you can add more parameters, follow the way we did it.
	// ** DO NOT CREATE A CONSTRUCTOR FOR MatchFragment **
	public static MatchFragment newInstance(StartOverCallbackListener activity, String host, int port, String command) {
		MatchFragment f = new MatchFragment();
		
		f.activity = activity;
		f.host = host;
		f.port = port;
		f.command = command;
		
		return f;
	}
	
	/**
	 * Called when the fragment will be displayed.
	 */
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		if (container == null) {
			return null;
		}
		
		View view = inflater.inflate(R.layout.match_fragment_layout, container, false);
		
		/**
		 * Register this fragment to be the OnClickListener for the startover
		 * button.
		 */
		view.findViewById(R.id.bu_start_over).setOnClickListener(this);
		
		status = (TextView) view.findViewById(R.id.tv_statusupdate);
		
		/**
		 * Launch the AsyncTask
		 */
		this.client = new Client();
		this.client.execute("");
		
		return view;
	}
	
	/**
	 * Callback function for the OnClickListener interface.
	 */
	@Override
	public void onClick(View v) {
		/**
		 * Close the AsyncTask if still running.
		 */
		try {
			this.client.close();
		} catch (IOException e) {
		}
		
		/**
		 * Notify the Activity.
		 */
		this.activity.onStartOver();
	}
	
	class Client extends AsyncTask<String, String, String> implements Closeable {
		
		/**
		 * NOTE: you can access MatchFragment field from this class:
		 * 
		 * Example: The statement in doInBackground will print the message in
		 * the Eclipse LogCat view.
		 */
		
		/**
		 * The system calls this to perform work in a worker thread and delivers
		 * it the parameters given to AsyncTask.execute()
		 */
		protected String doInBackground(String... params) {
			
			try {
				socket = new Socket(host, port);
				publishProgress("Connected Successfully!");
				
				PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
				out.println(command);
				publishProgress("Your information has been Sent!");
				
				String s;
				while (true) {
					s = in.readLine();
					if (s.startsWith("RESPONSE: "))
						out.println(":ACK");
					break;
				}
				
				return s;
			} catch (Exception e) {
				return "The server is not available.";
			}
		}
		
		public void close() throws IOException {
			if (socket != null) {
				socket.close();
				socket = null;
			}
			created = false;
			status.setText("");
		}
		
		/**
		 * The system calls this to perform work in the UI thread and delivers
		 * the result from doInBackground()
		 */
		
		// TODO: use the following method to update the UI.
		// ** DO NOT TRY TO CALL UI METHODS FROM doInBackground!!!!!!!!!! **
		
		/**
		 * Method executed just before the task.
		 */
		@Override
		protected void onPreExecute() {
			status.setText(String.format("The Server at the address %s uses the port %d", host, port));
		}
		
		/**
		 * Method executed once the task is completed.
		 */
		
		@Override
		protected void onPostExecute(String result) {
			if (result.startsWith("RESPONSE: ")) {
				result = result.substring(9);
				status.setText(String.format("%s%n Paired Successfully! Information:%n %s", status.getText(), result));
			} else {
				status.setText(String.format("%s%n%s", status.getText(), result));
			}
		}
		
		/**
		 * Method executed when progressUpdate is called in the doInBackground
		 * function.
		 */
		@Override
		protected void onProgressUpdate(String... result) {
			for (String s : result)
				status.setText(String.format("%s%n%s", status.getText(), s));
		}
	}
	
}
