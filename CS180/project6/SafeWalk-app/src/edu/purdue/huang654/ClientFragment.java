package edu.purdue.huang654;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.Spinner;

/**
 * This fragment is the "page" where the user inputs information about the
 * request, he/she wishes to send.
 *
 * @author YL
 */
public class ClientFragment extends Fragment implements OnClickListener {
	
	/**
	 * Activity which have to receive callbacks.
	 */
	private SubmitCallbackListener activity;
	private EditText name;
	private Spinner from;
	private Spinner to;
	private RadioButton t0,t1,t2;
	
	/**
	 * Creates a ProfileFragment
	 * 
	 * @param activity
	 *            activity to notify once the user click on the submit Button.
	 * 
	 *            ** DO NOT CREATE A CONSTRUCTOR FOR MatchFragment **
	 * 
	 * @return the fragment initialized.
	 */
	// ** DO NOT CREATE A CONSTRUCTOR FOR ProfileFragment **
	public static ClientFragment newInstance(SubmitCallbackListener activity) {
		ClientFragment f = new ClientFragment();
		
		f.activity = activity;
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
		
		View view = inflater.inflate(R.layout.client_fragment_layout, container, false);
		
		/**
		 * Register this fragment to be the OnClickListener for the submit
		 * Button.
		 */
		view.findViewById(R.id.bu_submit).setOnClickListener(this);
		
		this.name = (EditText) view.findViewById(R.id.et_name);
		
		this.t0 = (RadioButton)view.findViewById(R.id.rb_type0);
		this.t1 = (RadioButton)view.findViewById(R.id.rb_type1);
		this.t2 = (RadioButton)view.findViewById(R.id.rb_type2);
		
		
		this.from = (Spinner) view.findViewById(R.id.sp_from);
		ArrayAdapter<CharSequence> fromadapter = ArrayAdapter.createFromResource(this.getActivity(), R.array.from,
		        android.R.layout.simple_spinner_item);
		fromadapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		from.setAdapter(fromadapter);

		this.to = (Spinner) view.findViewById(R.id.sp_to);
		ArrayAdapter<CharSequence> toadapter = ArrayAdapter.createFromResource(this.getActivity(), R.array.to,
		        android.R.layout.simple_spinner_item);
		toadapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		to.setAdapter(toadapter);
		
		return view;
	}
	
	public String getName(String defaultVal) {
		return this.name != null ? name.getText().toString() : defaultVal;
	}
	
	public int getType() {
		if (t0.isChecked()) return 0;
		if (t1.isChecked()) return 1;
		if (t2.isChecked()) return 2;
		return 0;
	}
	
	public String getFrom() {
		return from.getSelectedItem().toString().split(":")[0];
	}
	
	public String getTo() {
		return to.getSelectedItem().toString().split(":")[0];		
	}
	
	/**
	 * Callback function for the OnClickListener interface.
	 */
	@Override
	public void onClick(View v) {
		this.activity.onSubmit();
	}
}
