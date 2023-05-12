package ustc.ddl.iofiles;
import ustc.ddl.cluster.StreamCluster;
import ustc.ddl.properties.GlobalConfig;

import java.util.*;
import java.io.*;



public class FileProcess {
    private final  StreamCluster streamCluster;
    private final HashMap<Integer, HashMap<Integer, Integer>> innerAndCutEdge;
    private final List<Integer> clusterList;
    private final int[] cluster;



    public FileProcess(StreamCluster streamCluster) {
        this.streamCluster = streamCluster;
        this.innerAndCutEdge = streamCluster.getInnerAndCutEdge();
        this.clusterList = streamCluster.getClusterList();
        this.cluster = streamCluster.getCluster();
    }

    public void savefile() {
        try {
            BufferedWriter out = new BufferedWriter(new FileWriter(GlobalConfig.getOutputGraphPath()+"streamCluster.txt"));
            System.out.println(GlobalConfig.getOutputGraphPath()+"streamCluster.txt");

            for(int i = 0; i < cluster.length; ++i) {
                out.write(cluster[i] + " ");
            }
            out.write("\n");

            for(int i = 0; i < clusterList.size(); ++i) {
                out.write(clusterList.get(i) + " ");
            }
            out.write("\n");

            innerAndCutEdge.forEach((key1, value1) -> {
                try {
                    out.write(key1.toString());
                    out.write(" ");
                } catch (IOException ex) {
                    //exception
                }
                value1.forEach((key2, value2) ->{
                    try {
                        out.write(key2.toString());
                        out.write(" ");
                        out.write(value2.toString());
                        out.write(" ");
                    }catch (IOException ex) {
                        //exception
                    }
                });
                try {
                    out.write("\n");
                } catch (IOException ex) {
                    //exception
                }
            });
            out.close();
        } catch (IOException ex) {
            //exception
        }
    }

    public void readfile() {

        try {
            BufferedReader in = new BufferedReader(new FileReader(GlobalConfig.getOutputGraphPath()+"streamCluster.txt"));

            String line;

            line = in.readLine();
            String[] precluster = line.split(" ");

            for(int i = 0; i < precluster.length; ++i) {
                this.cluster[i] = Integer.valueOf(precluster[i]).intValue();
            }


            line = in.readLine();
            String[] preclusterList = line.split(" ");
            for(int i = 0; i < preclusterList.length; ++i) {
                this.clusterList.add(Integer.valueOf(preclusterList[i]));
            }


            while((line = in.readLine()) != null){
                String[] preinnerAndCutEdge = line.split(" ");
                //  System.out.println(preinnerAndCutEdge.length);
                HashMap<Integer, Integer> temp = new HashMap<>();
                for(int i = 1; i < preinnerAndCutEdge.length; i+=2) {
//                if (!this.innerAndCutEdge.get(Integer.valueOf(preinnerAndCutEdge[i])).containsKey(Integer.valueOf(preinnerAndCutEdge[i + 1]));
//                  innerAndCutEdge.get(cluster[src]).put(cluster[dest], 0);
                    temp.put(Integer.valueOf(preinnerAndCutEdge[i]), Integer.valueOf(preinnerAndCutEdge[i + 1]));
                }
                this.innerAndCutEdge.put(Integer.valueOf(preinnerAndCutEdge[0]), temp);
            }
            in.close();
        }catch (IOException ex) {
            //exception
        }

    }

    public StreamCluster getstreamCluster() {
        this.streamCluster.changeCluster(this.cluster);
        this.streamCluster.changeClusterList(this.clusterList);
        return this.streamCluster;
    }

}
