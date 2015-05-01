/**
 * Project 5
 * @author He Huang, huang654, 814
 * @author Ge Yan, yan166, LN3
 */

import java.io.*;
import java.net.*;
import java.util.*;

public class SafeWalkServer implements Runnable {
	private int port;
	private ServerSocket serverSocket = null;
	private List<List> stringList = new ArrayList<List>();
	private List<Socket> socketList = new ArrayList<Socket>();
	
	// constructor
	public SafeWalkServer(int port) throws SocketException, IOException {
		this.serverSocket = new ServerSocket(port);
		this.port = port;
	}
	
	public SafeWalkServer() throws SocketException, IOException {
		this.serverSocket = new ServerSocket(0);
		this.port = this.serverSocket.getLocalPort();
	}
	
	/**
	 * Return the port number on which the server is listening.
	 */
	public int getLocalPort() {
		return port;
	}
	
	/**
	 * Start a loop to accept incoming connections.
	 */
	public void run() {
		while (true) {
			try {
				Socket clientSocket = serverSocket.accept();
				PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
				
				String inputLine = in.readLine();
				
				// Validity Check
				String locations = "CL50EELWSNPMUPUSH*";
				boolean flag = false;
				if (inputLine.charAt(0) == ':')
					flag = inputLine.equals(":LIST_PENDING_REQUESTS") || inputLine.equals(":RESET")
					        || inputLine.equals(":SHUTDOWN");
				else {
					String[] arrayStr = inputLine.split(",");
					if (arrayStr.length == 4) {
						if (!(arrayStr[1].equals(arrayStr[2]))
						        && ((locations.indexOf(arrayStr[1]) >= 0) && !arrayStr[1].equals("*"))
						        && (locations.indexOf(arrayStr[2]) >= 0))
							flag = true;
					}
				}
				if (!flag) {
					out.println("ERROR: invalid request");
					clientSocket.setReuseAddress(true);
					clientSocket.close();
					in.close();
					out.close();
					continue;
				}
				
				// Handle Request
				// :LIST_PENDING_REQUESTS
				if (inputLine.equals(":LIST_PENDING_REQUESTS")) {
					out.println(stringList.toString());
					clientSocket.setReuseAddress(true);
					clientSocket.close();
					in.close();
					out.close();
					continue;
				}
				
				// :RESET
				if (inputLine.equals(":RESET")) {
					for (int i = 0; i < socketList.size(); i++) {
						PrintWriter tempout = new PrintWriter(socketList.get(i).getOutputStream(), true);
						tempout.println("ERROR: connection reset");
						socketList.get(i).close();
						tempout.close();
					}
					socketList.clear();
					stringList.clear();
					out.println("RESPONSE: success");
					clientSocket.setReuseAddress(true);
					clientSocket.close();
					in.close();
					out.close();
					continue;
				}
				
				// :SHUTDOWN
				if (inputLine.equals(":SHUTDOWN")) {
					for (int i = 0; i < socketList.size(); i++) {
						PrintWriter tempout = new PrintWriter(socketList.get(i).getOutputStream(), true);
						tempout.println("ERROR: connection reset");
						socketList.get(i).close();
						tempout.close();
					}
					out.println("RESPONSE: success");
					serverSocket.setReuseAddress(true);
					serverSocket.close();
					break;
				}
				
				// Requests
				String[] arrayStr = inputLine.split(",");
				boolean check = false;
				for (int i = 0; i < stringList.size(); i++) {
					if (stringList.get(i).get(1).equals(arrayStr[1])) {
						if ((stringList.get(i).get(2).equals(arrayStr[2]) || (stringList.get(i).get(2).equals("*") || arrayStr[2]
						        .equals("*")))) {
							if (!(stringList.get(i).get(2).equals("*") && arrayStr[2].equals("*"))) {
								check = true;
								PrintWriter tempout = new PrintWriter(socketList.get(i).getOutputStream(), true);
								List<String> list = Arrays.asList(arrayStr);
								tempout.printf("RESPONSE: %s", list.toString());
								out.printf("RESPONSE: %s", stringList.get(i).toString());
								socketList.get(i).setReuseAddress(true);
								socketList.get(i).close();
								socketList.remove(i);
								stringList.remove(i);
								clientSocket.setReuseAddress(true);
								clientSocket.close();
								in.close();
								out.close();
								tempout.close();
								continue;
							}
						}
					}
				}
				if (!check) {
					stringList.add(Arrays.asList(arrayStr));
					socketList.add(clientSocket);
					continue;
				}
			} catch (IOException e) {
			}
		}
	}
	
	public static void main(String[] args) throws SocketException, IOException {
		SafeWalkServer s = null;
		if (args.length == 0) {
			s = new SafeWalkServer();
			System.out.println("Port not specified. Using free port " + s.port + ".");
		}
		if (args.length == 1) {
			int port = Integer.parseInt(args[0]);
			if (port < 1025 || port > 65535)
				System.out.println("Error: Port not vaild.");
			else
				s = new SafeWalkServer(port);
		}
		if (args.length > 1)
			System.out.println("Error: Please input only one port number.");
		if (s != null) {
			Thread st = new Thread(s);
			st.start();
		}
	}
	
}
