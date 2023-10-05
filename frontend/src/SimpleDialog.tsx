import * as React from 'react';
import Avatar from '@mui/material/Avatar';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemAvatar from '@mui/material/ListItemAvatar';
import ListItemText from '@mui/material/ListItemText';
import DialogTitle from '@mui/material/DialogTitle';
import Dialog from '@mui/material/Dialog';
import PersonIcon from '@mui/icons-material/Person';
import { blue } from '@mui/material/colors';
import { Chip, ListItemIcon } from '@mui/material';


export interface SimpleDialogProps {
    open: boolean;
    id: number;
    onClose: (value: number) => void;
}

export default function SimpleDialog(props: SimpleDialogProps) {
    const { id, open, onClose } = props;
    const [data, setData] = React.useState<any[]>([]);

    const fetchData = () => {
        fetch('http://127.0.0.1:5000/get_data/' + id)
            .then((response) => response.json())
            .then((actualData) => {
                console.log(actualData);
                setData(actualData.results);
                console.log(data);
            })
            .catch((err) => {
                console.log(err.message);
            });
    };

    const handleClose = () => {
        onClose(id);
    };

    React.useEffect(() => {
        fetchData();
    }, []);

    return (
        <Dialog open={open} onClose={handleClose}>
            <DialogTitle>Data from device</DialogTitle>
            <List sx={{ pt: 0 }}>
                {data.map((row, idx) => (
                    <ListItem disableGutters>
                        <Chip label={row.type} color="info"/>
                        <ListItemText primary={row.sensor} />
                        <ListItemText primary={row.value} />
                    </ListItem>
                ))}
            </List>
        </Dialog>
    );
}
