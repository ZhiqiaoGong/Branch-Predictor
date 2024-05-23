import matplotlib.pyplot as plt
import pandas as pd

# Data from the user's results
data = {
    'Benchmark': ['fp1', 'fp2', 'int1', 'int2', 'mm1', 'mm2'],
    'G-Share': [0.825, 1.678, 13.839, 0.420, 6.696, 10.138],
    'Tournament': [0.990, 2.970, 12.624, 0.426, 2.617, 8.482],
    'Custom': [0.822, 0.962, 7.547, 0.305, 2.160, 7.481]
}

# Creating a DataFrame
df = pd.DataFrame(data)

# Setting the positions and width for the bars
pos = list(range(len(df['Benchmark'])))
width = 0.25

# Plotting the bars
fig, ax = plt.subplots(figsize=(10, 6))

# Create a bar with G-Share data,
# in position pos,
plt.bar(pos, 
        df['G-Share'], 
        width, 
        alpha=0.5, 
        color='#FF0000', 
        label=df['Benchmark'][0]) 

# Create a bar with Tournament data,
# in position pos + some width buffer,
plt.bar([p + width for p in pos], 
        df['Tournament'],
        width, 
        alpha=0.5, 
        color='#00FF00', 
        label=df['Benchmark'][1]) 

# Create a bar with Custom data,
# in position pos + some width buffer,
plt.bar([p + width*2 for p in pos], 
        df['Custom'], 
        width, 
        alpha=0.5, 
        color='#0000FF', 
        label=df['Benchmark'][2]) 

# Set the y axis label
ax.set_ylabel('Misprediction Rate (%)')

# Set the chart's title
ax.set_title('Branch Predictor Performance Comparison')

# Set the position of the x ticks
ax.set_xticks([p + 1.5 * width for p in pos])

# Set the labels for the x ticks
ax.set_xticklabels(df['Benchmark'])

# Setting the x-axis and y-axis limits
plt.xlim(min(pos)-width, max(pos)+width*4)
plt.ylim([0, max(df['G-Share'] + df['Tournament'] + df['Custom'])] )

# Adding the legend and showing the plot
plt.legend(['G-Share', 'Tournament', 'Custom'], loc='upper left')
plt.grid()
plt.show()
