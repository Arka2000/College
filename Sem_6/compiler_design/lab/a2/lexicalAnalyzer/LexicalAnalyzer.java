package lexicalAnalyzer;

import java.io.*;
import java.util.*;

public class LexicalAnalyzer {

	private static ArrayList<String> lines = new ArrayList<String>();
	// An array of strings to hold lines in the input file
	

	private static String getDelimitersRegex() {
		// Function to read the input file
		File file = new File("lexicalAnalyzer/Dict/delimiters");
		String delimiters = "( )|(\t)|(\n)|(\r)";
		try {
			Scanner fin = new Scanner(file);
			while(fin.hasNext()) {
				//System.out.println(fin.nextLine());
				String s = fin.nextLine();
				delimiters += "|((?<=" + s + ")";
				delimiters += "|(?=" + s + "))";
				
			}
			fin.close();
		} catch(FileNotFoundException e) {
			System.out.println("Error:" + e.getMessage());
			e.printStackTrace();
			System.exit(0);
		}

		return delimiters;
	}

	private static void tokenize() {
		for(String line : lines) {
			String wordarr[] = line.split(getDelimitersRegex());
			List<String> words = new ArrayList<String>(
					Arrays.asList(wordarr));
			words.removeAll(Arrays.asList("",null));
			for(String word : words) {
				System.out.println(word);
			}
		}
	}

	private static void readFile(String filename) {
		// Function to read the input file
		File file = new File(filename);
		try {
			Scanner fin = new Scanner(file);
			while(fin.hasNext()) {
				lines.add(fin.nextLine());
			}
			fin.close();
		} catch(FileNotFoundException e) {
			System.out.println("Error:" + e.getMessage());
			e.printStackTrace();
			System.exit(0);
		}	
	}

	public static void run(String filename) {
		System.out.println("lexical analyzer");
		readFile(filename);
		tokenize();	
	}
}