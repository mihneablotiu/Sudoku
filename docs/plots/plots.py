import matplotlib.pyplot as plt

serial_times = [0.0031851428, 0.0035951206, 0.5572975745,
                0.4706260062, 5.1274108144, 20.6761871385,
                139.7189003841, 467.9522424586, 1401.462370869]

openmp_4_times = [0.0046315051, 0.007406522, 0.1548842032,
                  0.1522922515, 1.3582318907, 5.4080969035,
                  36.6536750511, 201.7334865385, 482.7871434006]

openmp_8_times = [0.0047685638, 0.0047842297, 0.1314964147,
                  0.1301761703, 1.22833682, 4.8701885277,
                  32.6806530817, 187.1013840224, 436.6537264977]

openmp_12_times = [0.0028510032, 0.0071315112, 0.0644110574,
                   0.0711446102, 0.5318935167, 2.0823440455,
                   13.8108026724, 76.8626663341, 181.6239390529]

openmp_16_times = [0.0038289204, 0.0058581936, 0.0573932556,
                   0.0627394187, 0.4337546315, 1.6385225678,
                   10.7409941158, 60.0013189449, 140.3267708696]

openmp_24_times = [0.0029955661, 0.0060287369, 0.0504381647,
                   0.0693937087, 0.3924811959, 1.4837162518,
                   9.6999624906, 54.8955989116, 128.1988547892]

openmp_32_times = [0.0029488234, 0.0074998727, 0.0496692132,
                   0.0776475289, 0.3671193442, 1.371172209,
                   8.938527078, 51.3252145969, 118.1494170929]

pthreads_4_times = [0.0041439677, 0.0065545901, 0.168125996,
                    0.1493966827, 1.394804696, 5.3962669296,
                    36.5773423876, 398.6984090833, 781.2925884341]

pthreads_8_times = [0.0032927816, 0.0050590801, 0.1351343049,
                    0.1302130904, 1.2342289957, 4.8683061248,
                    32.7414893427, 240.8771367888, 520.6060788012]

pthreads_12_times = [0.0078218066, 0.0060321472, 0.0801395204,
                     0.084082688, 0.5612507415, 2.092501358,
                     13.7595771596, 175.6228822981, 333.8503883906]

pthreads_16_times = [0.002913529, 0.0043860692, 0.066271717,
                     0.0833793521, 0.5060554845, 1.7623313378,
                     10.6945632291, 140.2415187297, 264.5064795733]

pthreads_24_times = [0.0053864899, 0.0073627256, 0.0561395517,
                     0.0786657133, 0.3997482022, 1.4744813571,
                     9.7215659385, 125.8867157064, 234.6090039257]

pthreads_32_times = [0.0058308754, 0.0096842625, 0.0522559423,
                     0.0836562189, 0.369622276, 1.3666121643,
                     8.8946004589, 122.5272429433, 222.6252505997]

rule_based_times = [0.0033583535, 0.0030077678, 0.0502740291,
                    0.4345337434, 0.0658566685, 1.7467218292,
                    4.8065607853, 4.6107359587, 50.7825296859]


# We make a time plot for each (serial vs openmp_x_threads vs pthreads_x_threads) combination where
# on the x-axis we have the test number and on the y-axis we have the time in seconds. The tests
# have the average times in each list in the following order: 4, 1, 2, 3, 5, 6, 7, 8, 9.
def generate_time_plot(x, rule_based=False):
    test_order = [1, 2, 3, 4, 5, 6, 7, 8, 9]

    openmp_possible_times = ['openmp_4_times', 'openmp_8_times', 'openmp_12_times',
                             'openmp_16_times', 'openmp_24_times', 'openmp_32_times']

    openmp_current_string = 'openmp_' + str(x) + '_times'
    openmp_current_times = filter(lambda time: time == openmp_current_string, openmp_possible_times)
    openmp_times = globals()[list(openmp_current_times)[0]]

    pthreads_possible_times = ['pthreads_4_times', 'pthreads_8_times', 'pthreads_12_times',
                               'pthreads_16_times', 'pthreads_24_times', 'pthreads_32_times']
    pthreads_current_string = 'pthreads_' + str(x) + '_times'
    pthreads_current_times = filter(lambda time: time == pthreads_current_string, pthreads_possible_times)
    pthreads_times = globals()[list(pthreads_current_times)[0]]

    plt.figure(figsize=(10, 6))
    plt.plot(test_order, serial_times, marker='o', label='Serial')
    plt.plot(test_order, openmp_times, marker='o', label=f'OpenMP {x} Threads')
    plt.plot(test_order, pthreads_times, marker='o', label=f'Pthreads {x} Threads')

    if rule_based:
        plt.plot(test_order, rule_based_times, marker='o', label='Rule Based')

    plt.xlabel('Test Number')
    plt.ylabel('Time (seconds)')
    plt.title(f'Time Plot for Serial vs OpenMP vs Pthreads with {x} Threads')

    plt.xticks(test_order)
    plt.yticks(range(0, int(max(max(serial_times), max(openmp_times), max(pthreads_times))) + 1, 50))
    plt.legend()

    if rule_based:
        output_directory = 'rule_based_included/time_plots/'
        output_filename = f'rule_based_time_plot_{x}_threads.png'
        plt.savefig(output_directory + output_filename)
    else:
        output_directory = 'time_plots/'
        output_filename = f'time_plot_{x}_threads.png'
        plt.savefig(output_directory + output_filename)

    plt.close()

def generate_speedup_plot(x):
    test_order = [1, 2, 3, 4, 5, 6, 7, 8, 9]

    openmp_possible_times = ['openmp_4_times', 'openmp_8_times', 'openmp_12_times',
                             'openmp_16_times', 'openmp_24_times', 'openmp_32_times']

    openmp_current_string = 'openmp_' + str(x) + '_times'
    openmp_current_times = filter(lambda time: time == openmp_current_string, openmp_possible_times)
    openmp_times = globals()[list(openmp_current_times)[0]]
    openmp_speedup = [serial_times[i] / openmp_times[i] for i in range(len(serial_times))]

    pthreads_possible_times = ['pthreads_4_times', 'pthreads_8_times', 'pthreads_12_times',
                               'pthreads_16_times', 'pthreads_24_times', 'pthreads_32_times']
    pthreads_current_string = 'pthreads_' + str(x) + '_times'
    pthreads_current_times = filter(lambda time: time == pthreads_current_string, pthreads_possible_times)
    pthreads_times = globals()[list(pthreads_current_times)[0]]
    pthread_speedup = [serial_times[i] / pthreads_times[i] for i in range(len(serial_times))]

    plt.figure(figsize=(10, 6))
    plt.plot(test_order, [1] * len(test_order), marker='o', label='Serial')
    plt.plot(test_order, openmp_speedup, marker='o', label=f'OpenMP {x} Threads')
    plt.plot(test_order, pthread_speedup, marker='o', label=f'Pthreads {x} Threads')

    plt.xlabel('Test Number')
    plt.ylabel('Speedup')
    plt.title(f'Speedup Plot for OpenMP and Pthreads with {x} Threads')

    plt.xticks(test_order)
    plt.legend()

    output_directory = 'speedup_plots/'
    output_filename = f'speedup_plot_{x}_threads.png'
    plt.savefig(output_directory + output_filename)

    plt.close()

def generate_rule_based_speedup_plot(x):
    test_order = [1, 2, 3, 4, 5, 6, 7, 8, 9]

    openmp_possible_times = ['openmp_4_times', 'openmp_8_times', 'openmp_12_times',
                             'openmp_16_times', 'openmp_24_times', 'openmp_32_times']

    openmp_current_string = 'openmp_' + str(x) + '_times'
    openmp_current_times = filter(lambda time: time == openmp_current_string, openmp_possible_times)
    openmp_times = globals()[list(openmp_current_times)[0]]
    openmp_speedup = [serial_times[i - 1] / openmp_times[i - 1] for i in test_order]

    rule_based_speedup = [openmp_times[i - 1] / rule_based_times[i - 1] for i in test_order]

    plt.figure(figsize=(10, 6))
    plt.plot(test_order, [1] * len(test_order), marker='o', label='OpenMP')
    plt.plot(test_order, rule_based_speedup, marker='o', label='Rule Based Speedup')

    plt.xlabel('Test Number')
    plt.ylabel('Speedup')
    plt.title(f'Speedup Plot for Rule Based over OpenMP with {x} Threads')

    plt.xticks(test_order)
    plt.legend()

    output_directory = 'rule_based_included/speedup_plots/'
    output_filename = f'rule_based_speedup_plot_{x}_threads.png'
    plt.savefig(output_directory + output_filename)

    plt.close()

def generate_openmp_speedup_plot():
    openmp_possible_times = ['openmp_4_times', 'openmp_8_times', 'openmp_12_times',
                             'openmp_16_times', 'openmp_24_times', 'openmp_32_times']
    
    graph_data = []

    for i in range(5, 9):
        current_graph_data = []
        for time in openmp_possible_times:
            openmp_speedup = serial_times[i] / globals()[time][i]
            current_graph_data.append(openmp_speedup)
        
        graph_data.append(current_graph_data)

    possible_threads = [4, 8, 12, 16, 24, 32]
    i = 6

    for current_graph_data in graph_data:
        plt.figure(figsize=(10, 6))
        plt.plot(possible_threads, current_graph_data, marker='o', label='Speedup')

        plt.xlabel('Number of threads')
        plt.ylabel('Speedup')
        plt.title(f'Speedup for OpenMP test {i}')

        plt.xticks(possible_threads)
        plt.legend()

        output_directory = 'test_plots/openMP/'
        output_filename = f'openmp_test_{i}_plot.png'
        plt.savefig(output_directory + output_filename)

        plt.close()
        i = i + 1


def generate_pthreads_speedup_plot():
    pthreads_possible_times = ['pthreads_4_times', 'pthreads_8_times', 'pthreads_12_times',
                               'pthreads_16_times', 'pthreads_24_times', 'pthreads_32_times']
    
    graph_data = []

    for i in range(5, 9):
        current_graph_data = []
        for time in pthreads_possible_times:
            openmp_speedup = serial_times[i] / globals()[time][i]
            current_graph_data.append(openmp_speedup)
        
        graph_data.append(current_graph_data)

    possible_threads = [4, 8, 12, 16, 24, 32]
    i = 6

    for current_graph_data in graph_data:
        plt.figure(figsize=(10, 6))
        plt.plot(possible_threads, current_graph_data, marker='o', label='Speedup')

        plt.xlabel('Number of threads')
        plt.ylabel('Speedup')
        plt.title(f'Speedup for Pthreads test {i}')

        plt.xticks(possible_threads)
        plt.legend()

        output_directory = 'test_plots/pthreads/'
        output_filename = f'pthreads_test_{i}_plot.png'
        plt.savefig(output_directory + output_filename)

        plt.close()
        i = i + 1

def generate_plots():
    possible_threads = [4, 8, 12, 16, 24, 32]
    for thread in possible_threads:
        generate_time_plot(thread)
        generate_speedup_plot(thread)
        generate_time_plot(thread, rule_based=True)
        generate_rule_based_speedup_plot(thread)
    generate_openmp_speedup_plot()
    generate_pthreads_speedup_plot()


generate_plots()
