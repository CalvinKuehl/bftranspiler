fn main() {
	use std::env;
	use std::fs;
	use std::io::Write;

	// get each file
	let argv: Vec<String> = env::args().collect(); // learn more about std::env::args() and collect()
	let outfilename: &str = &argv[1];
	let _infile: String = fs::read_to_string( &argv[2] ).expect("Could not open input file");

	// reset the output file
	let _ = fs::remove_file(outfilename);
	let mut outfile = fs::File::create(outfilename).expect("Could not open output file");

	// write the starter code for the assembly
	outfile.write("section .text\nglobal _start\n_start:".as_bytes()).expect("Could not edit file");
	
	// write the body of the code (what is defined by the brainfuck code)
	let givenchar: char = 129u8 as char;
	loop {
		if givenchar as i8 == -1 {
			break;
		}
	}

	// write the standard system exit call
	outfile.write("\n\tmov rax, 0x3c\n\tsyscall".as_bytes()).expect("Could not edit file");
}