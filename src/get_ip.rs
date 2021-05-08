use std::{collections::BTreeMap, net::Ipv4Addr, net::Ipv6Addr, str::FromStr};

fn hosts_to_cidr_v4(hosts: usize) -> u8 {
    let f = hosts as f64;
    let f = f.log2() as u8;
    32u8 - f
}

pub fn cidr_to_hosts_v4(cidr: &u8) -> usize {
    2usize.pow((32 - cidr) as u32)
}

pub fn cidr_to_mask_v4(cidr: &u8) -> Ipv4Addr {
    let mask = !((1u32 << (32 - cidr)) - 1);
    let mask = mask.to_be_bytes();
    Ipv4Addr::new(mask[0], mask[1], mask[2], mask[3])
}

pub fn get_first_ip_v4(ipv4: &Ipv4Addr, cidr: &u8) -> u32 {
    let mask = cidr_to_mask_v4(cidr);
    let mask = u32::from(mask);
    let be = u32::from(*ipv4);
    be & mask
}

pub fn get_ip_apnic_v4(data: &str) -> BTreeMap<Ipv4Addr, u8> {
    let mut map = BTreeMap::new();
    let lines = data.lines();

    for line in lines {
        let sp: Vec<&str> = line.split('|').collect();
        if sp.len() > 4 && sp[0] == r"apnic" && sp[1] == r"CN" && sp[2] == r"ipv4" {
            map.insert(
                Ipv4Addr::from_str(sp[3]).unwrap(),
                hosts_to_cidr_v4(sp[4].parse::<usize>().unwrap()),
            );
        }
    }

    map
}

pub fn get_ip_apnic_v6(data: &str) -> BTreeMap<Ipv6Addr, u8> {
    let mut map = BTreeMap::new();
    let lines = data.lines();

    for line in lines {
        let sp: Vec<&str> = line.split('|').collect();
        if sp.len() > 4 && sp[0] == r"apnic" && sp[1] == r"CN" && sp[2] == r"ipv6" {
            map.insert(
                Ipv6Addr::from_str(sp[3]).unwrap(),
                sp[4].parse::<u8>().unwrap(),
            );
        }
    }

    map
}

pub fn get_ip_ipip_net_v4(data: &str) -> BTreeMap<Ipv4Addr, u8> {
    let mut map = BTreeMap::new();
    let lines = data.lines();

    for line in lines {
        let sp: Vec<&str> = line.split('/').collect();
        if sp.len() == 2 {
            map.insert(
                Ipv4Addr::from_str(sp[0]).unwrap(),
                sp[1].parse::<u8>().unwrap(),
            );
        }
    }

    map
}
