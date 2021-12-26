use std::fs::File;
use std::io::{BufRead, BufReader};
use std::io::{Write, BufWriter};


fn main() {
    let _filename = "src/main.rs";
    // Open the file in read-only mode (ignoring errors).
    let file = File::open("input_3.txt").unwrap();
    let reader = BufReader::new(file);
    // Read the file line by line using the lines() iterator from std::io::BufRead.
    let outfile = std::fs::File::create("output_3.txt").expect("create failed");
    let mut outfile = BufWriter::new(outfile);
    for (index , line) in reader.lines().enumerate() {	
        let line = line.unwrap(); // Ignore errors.
        // Show the line and its number.
	let num: Vec<&str> = line.split_whitespace().collect();
	if index==0{
		continue;
	}
	else{
		let _a :i64 = num[0].trim().parse().unwrap();
		let mut int_arr :Vec<i64> = Vec::new();
		let mut integer:i64;
		for s in num{
			integer = s.parse().unwrap();
			int_arr.push(integer);
		}
		
		bubble_sort(&mut int_arr);
		if int_arr[0]< -100000 || int_arr[int_arr.len()-1]>100000{
			write!(outfile,"#{} -1\n",index).ok();
		}
		else{
		let mut x:i64=int_arr[1];
		let mut y:i64=int_arr[2];
		for i in 1..int_arr.len() {
			if x*x > int_arr[i]*int_arr[i]{
				y=x;
				x=int_arr[i];
			}
			if y*y > int_arr[i]*int_arr[i] && x*x < int_arr[i]*int_arr[i] {
				y = int_arr[i]
			}
		}
		//println!("#{} {:?}", index, int_arr);
		println!("#{} {} {}", index, x, y);
		write!(outfile, "#{} {} {}\n", index, x,y).ok();
        	}
        }
   }
}

pub fn bubble_sort<T: Ord>(arr: &mut [T]) {
    for i in 0..arr.len() {
        for j in 1..arr.len() - 1 - i {
            if arr[j] > arr[j + 1] {
                arr.swap(j, j + 1);
            }
        }
    }
}
