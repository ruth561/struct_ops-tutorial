// SPDX-License-Identifier: GPL-2.0
mod bpf;
use bpf::*;

use libbpf_rs::skel::*;
use std::mem::MaybeUninit;

fn main() {
    let mut open_object = MaybeUninit::uninit();
    let skel_builder = ExampleSkelBuilder::default();
    let open_skel = skel_builder.open(&mut open_object).unwrap();
    let mut skel = open_skel.load().unwrap();
    let _link = skel.maps.my_ops_sample.attach_struct_ops().unwrap();
    println!("Successfully attached bpf program!");

    loop {
        let duration = std::time::Duration::from_secs(100);
        std::thread::sleep(duration);
    }
}
