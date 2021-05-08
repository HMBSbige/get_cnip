use std::collections::BTreeSet;

pub fn get_domains(data: &str) -> BTreeSet<String> {
    let mut set = BTreeSet::new();
    let lines = data.lines();

    for line in lines {
        let line = line.trim();
        if line.starts_with('#') {
            continue;
        }
        let sp: Vec<&str> = line.split('/').collect();
        if sp.len() == 3 && sp[0] == r"server=" {
            let domain = sp[1].trim().to_ascii_lowercase();
            if !domain.is_empty() {
                set.insert(domain);
            }
        }
    }

    set
}
