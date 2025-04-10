/*
fn add1(let value: &mut u8){
	*value+=1;
}
*/

fn main() {
	use std::env;
	use std::fs;
	use std::io::Write;

	// get each file
	let argv: Vec<String> = env::args().collect(); // learn more about std::env::args() and collect()
	let outfilename: &str = &argv[1];
	let infile: String = fs::read_to_string( &argv[2] ).expect("Could not open input file");

	// reset the output file
	let _ = fs::remove_file(outfilename);
	let mut outfile = fs::File::create(outfilename).expect("Could not open output file");

	// write the starter code for the assembly
	outfile.write("section .text\nglobal _start\n_start:\n\tinc rsp".as_bytes()).expect("Could not edit file");
	
	// write the body of the code (what is defined by the brainfuck code)
	let _lastchar: char = 'a';//the value does not matter as long as it is not a brainfuck command
	let mut bjmpnum: u8 = 0;
	let mut ejmpnum: u8 = 0;
	for (_i, givenchar) in infile.chars().enumerate() {
		match givenchar as u8 {
			43 => outfile.write("\n\tinc byte [rsp]".as_bytes()).expect("Could not edit file"),
			44 => outfile.write("\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x0\n\tsyscall".as_bytes()).expect("Could not edit file"),
			45 => outfile.write("\n\tdec byte [rsp]".as_bytes()).expect("Could not edit file"),
			46 => outfile.write("\n\tmov rsi, rsp\n\tmov rdx, 1\n\tmov rdi, 0\n\tmov rax, 0x1\n\tsyscall".as_bytes()).expect("Could not edit file"),
			60 => outfile.write("\n\tdec rsp".as_bytes()).expect("Could not edit file"),
			62 => outfile.write("\n\tinc rsp".as_bytes()).expect("Could not edit file"),
			91 => {
				bjmpnum+=1;
				outfile.write(format!("\n\tcmp byte [rsp], 0\n\tje ej{}\n\tbj{}:", bjmpnum, bjmpnum).as_bytes()).expect("Could not edit file")
			}
			93 => {
				ejmpnum+=1;
				outfile.write(format!("\n\tcmp byte [rsp], 0\n\tjnz bj{}\n\tej{}:", ejmpnum, ejmpnum).as_bytes()).expect("Could not edit file")
			}
			_ => todo!()
		};
	}

	// write the standard system exit call
	outfile.write("\n\tmov rax, 0x3c\n\tsyscall".as_bytes()).expect("Could not edit file");
}