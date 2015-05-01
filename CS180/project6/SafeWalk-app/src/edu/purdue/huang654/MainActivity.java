package edu.purdue.huang654;

import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.FragmentTransaction;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements SubmitCallbackListener, StartOverCallbackListener {
	
	/**
	 * The ClientFragment used by the activity.
	 */
	private ClientFragment clientFragment;
	
	/**
	 * The ServerFragment used by the activity.
	 */
	private ServerFragment serverFragment;
	
	/**
	 * UI component of the ActionBar used for navigation.
	 */
	private Button left;
	private Button right;
	private TextView title;


	/**
	 * Called once the activity is created.
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main_layout);
		
		this.clientFragment = ClientFragment.newInstance(this);
		this.serverFragment = ServerFragment.newInstance();
		
		FragmentTransaction ft = getFragmentManager().beginTransaction();
		ft.add(R.id.fl_main, this.clientFragment);
		ft.commit();
	}
	
	/**
	 * Creates the ActionBar: - Inflates the layout - Extracts the components
	 */
	@SuppressLint("InflateParams")
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		final ViewGroup actionBarLayout = (ViewGroup) getLayoutInflater().inflate(R.layout.action_bar, null);
		
		// Set up the ActionBar
		final ActionBar actionBar = getActionBar();
		actionBar.setDisplayShowHomeEnabled(false);
		actionBar.setDisplayShowTitleEnabled(false);
		actionBar.setDisplayShowCustomEnabled(true);
		actionBar.setCustomView(actionBarLayout);
		
		// Extract the UI component.
		this.title = (TextView) actionBarLayout.findViewById(R.id.tv_title);
		this.left = (Button) actionBarLayout.findViewById(R.id.bu_left);
		this.right = (Button) actionBarLayout.findViewById(R.id.bu_right);
		this.right.setVisibility(View.INVISIBLE);
		
		return true;
	}
	
	/**
	 * Callback function called when the user click on the right button of the
	 * ActionBar.
	 * 
	 * @param v
	 */
	public void onRightClick(View v) {
		FragmentTransaction ft = getFragmentManager().beginTransaction();
		
		this.title.setText(this.getResources().getString(R.string.client));
		this.left.setVisibility(View.VISIBLE);
		this.right.setVisibility(View.INVISIBLE);
		ft.replace(R.id.fl_main, this.clientFragment);
		ft.commit();
	}
	
	/**
	 * Callback function called when the user click on the left button of the
	 * ActionBar.
	 * 
	 * @param v
	 */
	public void onLeftClick(View v) {
		FragmentTransaction ft = getFragmentManager().beginTransaction();
		
		this.title.setText(this.getResources().getString(R.string.server));
		this.left.setVisibility(View.INVISIBLE);
		this.right.setVisibility(View.VISIBLE);
		ft.replace(R.id.fl_main, this.serverFragment);
		ft.commit();
		
	}
	
	/**
	 * Callback function called when the user click on the submit button.
	 */
	@Override
	public void onSubmit() {
		boolean flag = true;
		// Server info
		String host = this.serverFragment.getHost(getResources().getString(R.string.default_host));
		int port = this.serverFragment.getPort(Integer.parseInt(getResources().getString(R.string.default_port)));
		
		// Client info
		String name = this.clientFragment.getName("Student");
		int type = this.clientFragment.getType();
		String from = this.clientFragment.getFrom();
		String to = this.clientFragment.getTo();
		
		// Check Server
		if ((host.indexOf(" ") > 0) || host.equals("")) {
			new AlertDialog.Builder(this).setMessage("Invalid Host.").setPositiveButton("OK", null).show();
			flag = false;
		}
		if (port < 1 || port > 65536) {
			new AlertDialog.Builder(this).setMessage("Invalid Port.").setPositiveButton("OK", null).show();
			flag = false;
		}
		// Check Client
		if ((name.indexOf(",") > 0) || name.equals("")) {
			new AlertDialog.Builder(this).setMessage("Invalid Name.").setPositiveButton("OK", null).show();
			flag = false;
		}
		if ((type != 0) && (type != 1) && (type != 2)) {
			new AlertDialog.Builder(this).setMessage("Invalid Preference.").setPositiveButton("OK", null).show();
			flag = false;
		}
		String locations = "CL50EELWSNPMUPUSH*";
		if ((locations.indexOf(from) < 0) && (from.equals("*"))) {
			new AlertDialog.Builder(this).setMessage("Invalid From.").setPositiveButton("OK", null).show();
			flag = false;
		}
		if (locations.indexOf(to) < 0) {
			new AlertDialog.Builder(this).setMessage("Invalid To.").setPositiveButton("OK", null).show();
			flag = false;
		}
		if (from.equals(to)) {
			new AlertDialog.Builder(this).setMessage("The same from and to.").setPositiveButton("OK", null).show();
			flag = false;
		}
		
		// Get command from client fragment
		String command = this.getResources().getString(R.string.default_command);
		if (flag)
			command = name + "," + from + "," + to + "," + String.valueOf(type);
		
		// Valid then jump to match screen.
		if (flag) {
			FragmentTransaction ft = getFragmentManager().beginTransaction();
			
			this.title.setText(getResources().getString(R.string.match));
			this.left.setVisibility(View.INVISIBLE);
			this.right.setVisibility(View.INVISIBLE);
			
			MatchFragment frag = MatchFragment.newInstance(this, host, port, command);
			
			ft.replace(R.id.fl_main, frag);
			ft.commit();
		}
	}
	
	/**
	 * Callback function call from MatchFragment when the user want to create a
	 * new request.
	 */
	@Override
	public void onStartOver() {
		onRightClick(null);
	}
	
}
