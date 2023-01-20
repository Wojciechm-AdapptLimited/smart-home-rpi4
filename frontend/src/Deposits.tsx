import * as React from 'react';
import Link from '@mui/material/Link';
import Typography from '@mui/material/Typography';
import Title from './Title'
import SimpleDialog from './SimpleDialog';

function preventDefault(event: React.MouseEvent) {
    event.preventDefault();
}

export default function Deposits(props: any) {
    const [open, setOpen] = React.useState(false);

    const handleClickOpen = () => {
        setOpen(true);
      };
    
      const handleClose = (value: number) => {
        setOpen(false);
      };

    return (
        <React.Fragment>
            <Title>{props.name}</Title>
            <Typography component="p" variant="h4">
                {props.d_type}
            </Typography>
            <Typography color="text.secondary" sx={{ flex: 1 }}>
            </Typography>
            <div>
                <Link color="primary" href="#" onClick={handleClickOpen}>
                    View data
                </Link>
            </div>
            <SimpleDialog id={props.id} open={open} onClose={handleClose} />
        </React.Fragment>
    );
}