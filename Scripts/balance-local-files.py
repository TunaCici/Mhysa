import os
import argparse

def count_lines(file_path : str):
    try:
        with open(file_path, 'r', encoding="utf-8") as file:
            return sum(1 for line in file)
    except Exception as e:
        print(f"error reading {file_path}: {e}")
        return 0

def load_balance(pairs : dict, people : int):
    sorted_pairs = sorted(pairs.items(), key=lambda x:x[1], reverse=True)

    people_files = [[] for _ in range(people)]
    people_loads = [0] * people

    for i in sorted_pairs:
        min_loaded = people_loads.index(min(people_loads))
        
        people_files[min_loaded].append(i)
        people_loads[min_loaded] += (i[1])
    
    return people_files, people_loads


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument("--folder", type=str, help="folder of files to balance",
                        required=True)
    parser.add_argument("--people", type=str, help="amount of people to assign",
                        required=True)

    args = parser.parse_args()

    # Files & their lines count
    pairs = {}

    if not os.path.isdir(args.folder):
        print(f"folder does not exist: ${args.folder}")

    for file in os.listdir(args.folder):
        full_path = os.path.join(args.folder, file)

        if os.path.isfile(full_path) and "_tr" in full_path:
            line_count = count_lines(full_path)
            pairs[file] = line_count

    # Balance & log statistics
    files, loads = load_balance(pairs, int(args.people))

    for idx, load in enumerate(loads):
        print(f"person {idx} - total lines: {load}")

        for i in files[idx]:
            print(f"{i[0]} - count: {i[1]}")
