import pandas as pd
import matplotlib.pyplot as plt
plt.switch_backend('Agg')
import seaborn as sns

# Load the dataset
file_path = "NumaBST.csv"
df = pd.read_csv(file_path)

# Rename relevant columns for easier access
df.columns = ["Date", "Time", "DS_name", "num_DS", "num_threads", "thread_config", "DS_config", "duration", "keyspace", "Op0", "Op1", "TotalOps"]
df = df[["num_DS", "DS_config", "thread_config", "TotalOps"]]

# Convert TotalOps to numeric, handling errors
df["TotalOps"] = pd.to_numeric(df["TotalOps"], errors='coerce')

# Function to plot TotalOps within each num_DS subset
def plot_graph(num_DS_value):
    df_filtered = df[df["num_DS"] == num_DS_value].copy()
    
    # Create the bar plot
    plt.figure(figsize=(8, 6))
    sns.barplot(data=df_filtered, x="DS_config", y="TotalOps", hue="thread_config")
    plt.title(f"TotalOps for num_DS = {num_DS_value}")
    plt.ylabel("TotalOps")
    plt.xlabel("DS Config")
    plt.legend(title="Thread Config")
    print(df_filtered.head())
    plt.show()

# Generate graphs for num_DS = 128 and num_DS = 1024
plot_graph(128)
plot_graph(1024)

