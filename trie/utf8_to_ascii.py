input_file = "el_quijote.txt"
output_file = "el_quijote_ascii.txt"

with open(input_file, "r", encoding="utf-8") as infile, open(output_file, "w", encoding="ascii", errors="ignore") as outfile:
  for line in infile: outfile.write(line)
