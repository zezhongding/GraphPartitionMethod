package ustc.ddl.thread;

import ustc.ddl.graph.Graph;
import ustc.ddl.cluster.StreamCluster;

import java.util.concurrent.Callable;

public class ClusterTask implements Callable<StreamCluster> {

    private final Graph graph;
    private final int taskId;

    public ClusterTask(Graph graph, int taskId) {
        this.graph = graph;
        this.taskId = taskId;
    }

    @Override
    public StreamCluster call() throws Exception {
        StreamCluster streamCluster = new StreamCluster(graph);
        streamCluster.startSteamCluster();
        return streamCluster;
    }
}
