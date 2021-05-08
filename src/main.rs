mod constants;
mod generate_file;
mod get_domains;
mod get_ip;
mod raw_data;

use constants::*;
use std::collections::BTreeSet;

#[tokio::main]
async fn main() {
    println!("正在下载数据...");

    let data = load_from_internet().await;
    let apnic_v4 = get_ip::get_ip_apnic_v4(&data.apnic);
    let apnic_v6 = get_ip::get_ip_apnic_v6(&data.apnic);
    let ipip_net_v4 = get_ip::get_ip_ipip_net_v4(&data.ipip_net);

    println!("Apnic v4: {} 条", apnic_v4.len());
    println!("Apnic v6: {} 条", apnic_v6.len());
    println!("ipip.net v4: {} 条", ipip_net_v4.len());

    generate_file::write_chn_ip(&ipip_net_v4, &apnic_v6);
    generate_file::write_add_route(&ipip_net_v4);
    generate_file::write_delete_route(&ipip_net_v4);

    let domains = get_domains::get_domains(&data.cn_domains);
    println!("共有域名: {} 条", domains.len());

    generate_file::write_chndomains(&domains);

    let empty_domains = BTreeSet::new();

    tokio::join!(
        generate_file::write_sscnall(&apnic_v4, &domains),
        generate_file::write_sscnip(&apnic_v4, &empty_domains),
        generate_file::write_sswhite(&apnic_v4, &domains),
        generate_file::write_sswhite_r(&apnic_v4, &domains)
    );

    generate_file::write_whitelist_acl(&ipip_net_v4, &domains);

    println!("done");
}

async fn load_from_internet() -> raw_data::RawData {
    let (a, b, c) = tokio::join!(
        get_string(APNIC_URL),
        get_string(IPIP_NET_URL),
        get_string(ACCELERATED_DOMAINS_CHINA_URL)
    );

    raw_data::RawData {
        apnic: a,
        ipip_net: b,
        cn_domains: c,
    }
}

async fn get_string(url: &str) -> String {
    reqwest::get(url).await.unwrap().text().await.unwrap()
}
