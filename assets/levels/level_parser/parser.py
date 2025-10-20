import sys

def add_commas_to_lines(input_file, output_file):
    try:
        with open(input_file, 'r') as infile:
            with open(output_file, 'w') as outfile:
                line_num = 0
                for line in infile:
                    if not line_num:
                        modified_line = f'{line.strip()},\n'
                    else:
                        modified_line = f',{line.strip()},\n'
                    outfile.write(modified_line)
                    line_num += 1
        print("File processed successfully!")
    except Exception as e:
        print(f"An error occurred: {e}")

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 3:
    print("Usage: python script.py <input_file_path> <output_file_path>")
else:
    input_file_path = sys.argv[1]
    output_file_path = sys.argv[2]
    add_commas_to_lines(input_file_path, output_file_path)
