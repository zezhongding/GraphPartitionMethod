package ustc.ddl.utility;

import ustc.ddl.properties.GlobalConfig;

import java.io.*;
import java.sql.SQLOutput;
import java.util.Collection;
import java.util.Hashtable;
import java.util.Properties;

public class FileConvert {
    public static void main(String[] args) throws IOException {
        Hashtable<Integer, Integer> ht = new Hashtable<>();
        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("newpartition/src/data/698.edges")));
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream("newpartition/src/data/D698.edges")));
        String line;
        int lineNum = 0;
        int id = 0;
        while((line = br.readLine()) != null){
            if (line.startsWith("#")) continue;
            String[] edgeValues = line.split(" ");
            int srcVid = Integer.parseInt(edgeValues[0]);
            int endVid = Integer.parseInt(edgeValues[1]);
            int newSrcVid;
            int newEndVid;
            if(ht.containsKey(srcVid)) {
                newSrcVid = ht.get(srcVid);
            } else {
                newSrcVid = id++;
                ht.put(srcVid, newSrcVid);
            }
            if(ht.containsKey(endVid)) {
                newEndVid = ht.get(endVid);
            } else {
                newEndVid = id++;
                ht.put(endVid, newEndVid);
            }
            if (lineNum != 0)  {
                bw.write("\n");
                bw.flush();
            }
            bw.write(newSrcVid + "\t" + newEndVid);
            bw.flush();
            lineNum++;
        }
        System.out.println("Number of Nodes: " + id);
        System.out.println("Number of Edges: " + lineNum);
        br.close();
        bw.close();
    }
}
