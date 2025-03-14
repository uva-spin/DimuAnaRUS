with open('list_good_spill.txt', 'r') as spill_file:
    spill_numbers = {line.split()[1].strip().lstrip('0') for line in spill_file}

with open('all_run_list_dst.txt', 'r') as list_file:
    files = list_file.readlines()

print("Spill numbers loaded:", spill_numbers)

matching_files = []
for file in files:
    file = file.strip()  
    print(f"Checking file: {file}")  
    
    parts = file.split('/')
    for part in parts:
        if 'spill_' in part:
            spill_number = part.split('_')[1].lstrip('0')  
            print(f"  Found spill number: {spill_number}")
            if spill_number in spill_numbers:
                matching_files.append(file) 
            break
if not matching_files:
    print("No matching files found.")

with open('list_dst.txt', 'w') as output_file:
    for file in matching_files:
        output_file.write(file + '\n')
print(f"Matching files have been written to 'list_dst.txt'")

