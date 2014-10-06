import java.util.*;

public class Solution {
    public static String reverseWords(String s) {
        List<String> wordList = new ArrayList<String>();
        String currentWord = "";
        String output = "";
        for (int i = 0; i < s.length(); i++) {
            System.out.println(s.charAt(i);
            if (s.charAt(i) != ' ') {
                currentWord = currentWord + s.charAt(i);
            } else {
                wordList.add(currentWord);
                currentWord = "";
            }
        }
        int if_first = 1;
        for (int j = wordList.size() - 1; j >= 0; j--) {
            if ((wordList.get(j) != "") && (if_first == 1)) {
                output += wordList.get(j);
                if_first = 0;
            } else if ((wordList.get(j) != "") && (if_first == 0)) {
                output = output + " " + wordList.get(j);
            }
        }
        return output;
    }

    public static void main(String[] args) {
        reverseWords("a");
    }
}