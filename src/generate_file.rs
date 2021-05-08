use std::{
    collections::BTreeMap,
    collections::BTreeSet,
    fs::{self, File},
    io::Write,
    net::Ipv4Addr,
    net::Ipv6Addr,
    path::Path,
};

extern crate serde;
use crate::constants::*;
use crate::get_ip;

const PATH: &str = r"output";

fn get_file(filename: &str) -> File {
    let path = Path::new(PATH);
    fs::create_dir_all(path).unwrap();

    let path = path.join(filename);
    File::create(path).unwrap()
}

pub fn write_chn_ip(ipv4: &BTreeMap<Ipv4Addr, u8>, ipv6: &BTreeMap<Ipv6Addr, u8>) {
    const FILENAME: &str = r"chn_ip.txt";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    for (ip, cidr) in ipv4 {
        let line = format!("{}/{}\n", ip, cidr);
        file.write_all(line.as_bytes()).unwrap();
    }

    for (ip, cidr) in ipv6 {
        let line = format!("{}/{}\n", ip, cidr);
        file.write_all(line.as_bytes()).unwrap();
    }
}

fn write_route(file: &mut File, ipv4: &BTreeMap<Ipv4Addr, u8>, prefix: &str) {
    for (ip, cidr) in ipv4 {
        let line = format!(
            "{} {} mask {} default METRIC default IF default\n",
            prefix,
            ip,
            get_ip::cidr_to_mask_v4(cidr)
        );
        file.write_all(line.as_bytes()).unwrap();
    }
}

pub fn write_add_route(ipv4: &BTreeMap<Ipv4Addr, u8>) {
    const FILENAME: &str = r"add.txt";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    write_route(&mut file, ipv4, r"add");
}

pub fn write_delete_route(ipv4: &BTreeMap<Ipv4Addr, u8>) {
    const FILENAME: &str = r"del.txt";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    write_route(&mut file, ipv4, r"delete");
}

pub fn write_chndomains(domains: &BTreeSet<String>) {
    const FILENAME: &str = r"chndomains.txt";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    for domain in domains {
        file.write_all(domain.as_bytes()).unwrap();
        file.write_all(b"\n").unwrap();
    }
}

async fn write_js(file: &mut File, str: &str) {
    let old_length = str.len();

    let params = [(r"input", str)];
    let client = reqwest::Client::new();
    let response = client
        .post(r"https://javascript-minifier.com/raw")
        .form(&params)
        .send()
        .await
        .unwrap();
    let content = response.text().await.unwrap();

    println!("{} => {}", old_length, content.len());

    file.write_all(content.as_bytes()).unwrap();
}

fn get_cnip_range(ipv4: &BTreeMap<Ipv4Addr, u8>) -> String {
    let mut str = String::with_capacity(100 * 1024); //100KB
    str.push_str("[\n{");

    let mut start_num = 0u32;
    let mut comma = r"";

    for (ip, cidr) in ipv4 {
        let first_ip = get_ip::get_first_ip_v4(ip, cidr);
        while first_ip >> 24 > start_num {
            start_num += 1;
            str.push_str(r"},{");
            comma = r"";
        }

        str.push_str(comma);
        str.push_str(r"0x");
        str.push_str(&format!("{:x}", first_ip >> 8));
        str.push(':');
        str.push_str(&(get_ip::cidr_to_hosts_v4(cidr) >> 8).to_string());
        comma = r",";
    }

    str.push_str("}\n];");
    str
}

fn get_cnip16_range(ipv4: &BTreeMap<Ipv4Addr, u8>) -> String {
    let mut str = String::with_capacity(20 * 1024); // 20KB
    str.push_str("{\n");

    let mut master_net_set = BTreeSet::new();

    for (ip, cidr) in ipv4 {
        let hosts = get_ip::cidr_to_hosts_v4(cidr);

        if hosts < 1 << 14 {
            let first_ip = get_ip::get_first_ip_v4(ip, cidr);
            master_net_set.insert(first_ip >> 14);
        }
    }

    for x in master_net_set {
        str.push_str(r"0x");
        str.push_str(&format!("{:x}", x));
        str.push_str(r":1,");
    }

    str.pop();
    str.push_str("\n};");
    str
}

fn get_pac_white_domains(domains: &BTreeSet<String>) -> String {
    let mut m = BTreeMap::new();
    for domain in domains {
        let last_dot_index = domain.rfind('.');
        if last_dot_index.is_none() {
            continue;
        }
        let last_dot_index = last_dot_index.unwrap();
        let second_level = &domain[..last_dot_index];
        let top_level = &domain[last_dot_index + 1..];
        if !second_level.is_empty() && !top_level.is_empty() {
            let a = m.get_mut(top_level);
            match a {
                None => {
                    let mut sub = BTreeMap::new();
                    sub.insert(second_level, 1);
                    m.insert(top_level, sub);
                }
                Some(value) => {
                    value.insert(second_level, 1);
                }
            };
        }
    }

    serde_json::to_string(&m).unwrap()
}

async fn write_sscn(file: &mut File, ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    let str = CNIP_TEMPLATE;
    let str = str.replace(r"__cnIpRange__", &get_cnip_range(ipv4));
    let str = str.replace(r"__cnIp16Range__", &get_cnip16_range(ipv4));
    let str = str.replace(r"__white_domains__", &get_pac_white_domains(domains));

    write_js(file, &str).await;
}

pub async fn write_sscnall(ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    const FILENAME: &str = r"ss_cnall.pac";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    write_sscn(&mut file, ipv4, domains).await;
}

pub async fn write_sscnip(ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    const FILENAME: &str = r"ss_cnip.pac";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    write_sscn(&mut file, ipv4, domains).await;
}

pub async fn write_sswhite(ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    const FILENAME: &str = r"ss_white.pac";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    let str = WHITE_TEMPLATE;
    let str = str.replace(r"__cnIpRange__", &get_cnip_range(ipv4));
    let str = str.replace(r"__cnIp16Range__", &get_cnip16_range(ipv4));
    let str = str.replace(r"__white_domains__", &get_pac_white_domains(domains));

    write_js(&mut file, &str).await;
}

pub async fn write_sswhite_r(ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    const FILENAME: &str = r"ss_white_r.pac";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    let str = WHITE_R_TEMPLATE;
    let str = str.replace(r"__cnIpRange__", &get_cnip_range(ipv4));
    let str = str.replace(r"__cnIp16Range__", &get_cnip16_range(ipv4));
    let str = str.replace(r"__white_domains__", &get_pac_white_domains(domains));

    write_js(&mut file, &str).await;
}

fn get_acl_ip(ipv4: &BTreeMap<Ipv4Addr, u8>) -> String {
    let mut str = String::with_capacity(100 * 1024); // 100 KB

    for (ip, cidr) in ipv4 {
        str.push_str(&format!("{}/{}\n", ip, cidr));
    }

    str
}

fn get_acl_domains(domains: &BTreeSet<String>) -> String {
    let mut str = String::with_capacity(2 * 1024 * 1024); // 2 MB

    for domain in domains {
        let domain = domain.replace(r".", r"\.");
        str.push_str(&format!(r"^(.*\.)?{}$", domain));
        str.push('\n');
    }

    str
}

pub fn write_whitelist_acl(ipv4: &BTreeMap<Ipv4Addr, u8>, domains: &BTreeSet<String>) {
    const FILENAME: &str = r"whitelist.acl";
    println!("正在生成 {}...", FILENAME);
    let mut file = get_file(FILENAME);

    let str = WHITELIST_ACL_TEMPLATE;
    let str = str.replace(r"__CNIP__", &get_acl_ip(ipv4));
    let str = str.replace(r"__white_domains__", &get_acl_domains(domains));

    file.write_all(str.as_bytes()).unwrap();
}
