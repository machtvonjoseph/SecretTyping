import pandas as pd
import matplotlib.pyplot as plt
plt.switch_backend('Agg')
import seaborn as sns

# Load the dataset
file_path = "NumaBST.csv"
df = pd.read_csv(file_path)

# Rename relevant columns for easier access
df.columns = ["Date", "Time", "DS_name", "num_DS", "num_threads", "thread_config", "DS_config", "duration", "keyspace", "Op0", "Op1", "TotalOps"]
df = df[["num_DS", "num_threads", "DS_config", "thread_config", "TotalOps"]]

# Convert TotalOps to numeric, handling errors
df["TotalOps"] = pd.to_numeric(df["TotalOps"], errors='coerce')

# Function to plot TotalOps within each num_DS and num_threads subset
def plot_graph(num_DS_value, num_threads_value):
    df_filtered = df[(df["num_DS"] == num_DS_value) & (df["num_threads"] == num_threads_value)].copy()
    
    # Create the bar plot
    plt.figure(figsize=(8, 6))
    sns.barplot(data=df_filtered, x="DS_config", y="TotalOps", hue="thread_config")
    plt.title(f"TotalOps for num_DS = {num_DS_value}, num_threads = {num_threads_value}")
    plt.ylabel("TotalOps")
    plt.xlabel("DS Config")
    plt.legend(title="Thread Config")
    plt.savefig(f"Graph_{num_DS_value}_{num_threads_value}.svg")

# Generate graphs for each combination
plot_graph(128, 20)
plot_graph(128, 40)
plot_graph(1024, 20)
plot_graph(1024, 40)